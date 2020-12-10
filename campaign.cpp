#include "campaign.h"

#include "gmnpaths.h"
#include "gmnutils.h"
#include "gmnserializable.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QMessageBox>
#include <QTextStream>

Campaign::Campaign(QObject *parent)
    : QObject(parent)
{

}

Campaign::Campaign(const Campaign &other)
    : QObject(other.parent()),
      campaignInfo(other.campaignInfo),
      path(other.path)
{

}

Campaign::Campaign(Campaign &&other)
    : QObject(other.parent()),
      campaignInfo(std::exchange(other.campaignInfo, CampaignInfo())),
      path(std::exchange(other.path, nullptr))
{

}

Campaign &Campaign::operator=(const Campaign &other)
{
    return *this = Campaign(other);
}

Campaign &Campaign::operator=(Campaign &&other)
{
    std::swap(campaignInfo, other.campaignInfo);
    std::swap(path, other.path);
    return *this;
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
    this->path = path;
    std::optional<QString> infoFile = GMN::loadJsonFile(path + "/" + GMN::infoFileName);
    if (!infoFile) {
        emit loadingFailed(QObject::tr("Unable to open info file"));
    }
    if (!campaignInfo.loadFromJson(infoFile.value())) {
        emit loadingFailed(QObject::tr("Information file of campaign is corrupted"));
    }

    emit succesfullyLoaded();
}

void Campaign::saveToFile(const std::unique_ptr<CharactersModel> &charactersModel)
{
    saveInfoFile();
    saveCharactersToFile(charactersModel);
}

void Campaign::saveInfoFile()
{
    QString savePath = path + "/" + GMN::infoFileName;
    QString content = campaignInfo.toJsonString();
    if (!GMN::saveToFile(savePath, content)) {
        emit savingFailed(tr("Unable to save information file of campaign!"));
    }
}

void Campaign::saveCharactersToFile(const std::unique_ptr<CharactersModel> &charactersModel)
{
    QString serializedCharacters = charactersModel ? charactersModel->toJsonString() : CharactersModel().toJsonString();
    QString savePath(path + "/" + GMN::charactersFileName);
    if (!GMN::saveToFile(savePath, serializedCharacters)) {
        emit savingFailed(tr("Unable to save characters."));
    }
}

std::unique_ptr<CharactersModel> Campaign::createCharactersModel()
{
    auto model = std::make_unique<CharactersModel>();
    std::optional<QString> charactersFile = GMN::loadJsonFile(path + "/" + GMN::charactersFileName);

    if (charactersFile) {
        model->loadFromJson(charactersFile.value().toUtf8());
    }

    return model;
}

void Campaign::clear()
{
    campaignInfo = CampaignInfo();
    path = QString();
}
