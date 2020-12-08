#include "campaign.h"

#include "gmnpaths.h"
#include "gmnutils.h"
#include "gmnserializable.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

Campaign::Campaign(QObject *parent)
    : QObject(parent)
{

}

void Campaign::createNewCampaign(const QString &name)
{
    clear();
    if (QFile::exists(GMN::campaignsFolder + "/" + name + "/" + GMN::infoFileName)) {
        emit loadingFailed("Project under given name already exists in default path.");
    }

    QDateTime currentDateTime = QDateTime::currentDateTime();
    campaignInfo.setCreationDate(currentDateTime);
    campaignInfo.setLastOpenedDate(currentDateTime);
    campaignInfo.setName(name);
    emit succesfullyLoaded();
}

void Campaign::loadFromName(const QString &name)
{
    loadFromPath(GMN::campaignsFolder + "/" + name);
}

void Campaign::loadFromPath(const QString &path)
{ 
    clear();
    std::optional<QString> infoFile = GMN::loadJsonFile(path + "/" + GMN::infoFileName);
    if (!infoFile) {
        emit loadingFailed(QObject::tr("Unable to open info file"));
    }
    if (!campaignInfo.loadFromJson(infoFile.value())) {
        emit loadingFailed(QObject::tr("Information file of campaign is corrupted"));
    }

    std::optional<QString> charactersFile = GMN::loadJsonFile(path + "/" + GMN::charactersFileName);
    if (charactersFile) {
        characters.loadFromJson(charactersFile.value().toUtf8());
    }
    emit succesfullyLoaded();
}

void Campaign::passCharactersModel(QAbstractItemView *itemView)
{
    if (itemView) {
        itemView->setModel(&characters);
    }
}

void Campaign::clear()
{
    characters.clear();
}
