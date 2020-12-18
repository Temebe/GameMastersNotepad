#include "campaign.h"

#include "gmnpaths.h"
#include "gmnutils.h"
#include "gmnserializable.h"
#include "gmnobjectmodel.tpp"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QTextStream>
#include <QDir>

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
    path = GMN::campaignsFolder + "/" + name;
    QString newCampaignInfoPath = path + "/" + GMN::infoFileName;
    if (QFile::exists(newCampaignInfoPath)) {
        emit loadingFailed(tr("Project under given name already exists in default path."),
                           tr("File %1 already exists").arg(newCampaignInfoPath));
        return;
    }

    QDateTime currentDateTime = QDateTime::currentDateTime();
    campaignInfo.setCreationDate(currentDateTime);
    campaignInfo.setLastOpenedDate(currentDateTime);
    campaignInfo.setName(name);
    saveToFile();
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

    campaignInfo.setLastOpenedDate(QDateTime::currentDateTime());
    saveInfoFile();
    emit succesfullyLoaded();
}

void Campaign::saveToFile(const CharacterModelTypePtr &charactersModel, const LocationModelTypePtr &locationsModel)
{
    QDir().mkpath(path);
    QDir().mkpath(path + "/" + GMN::imagesFolderName);
    saveInfoFile();
    saveCharactersToFile(charactersModel);
    saveLocationsToFile(locationsModel);
}

void Campaign::saveInfoFile()
{
    QString savePath = path + "/" + GMN::infoFileName;
    QString content = campaignInfo.toJsonString();
    if (!GMN::saveToFile(savePath, content)) {
        emit savingFailed(tr("Unable to save information file of campaign!"));
    }
}

void Campaign::saveCharactersToFile(const CharacterModelTypePtr &charactersModel)
{
    QString serializedCharacters = charactersModel ? charactersModel->toJsonString() : GMNObjectModel<Character>().toJsonString();
    QString savePath(path + "/" + GMN::charactersFileName);
    if (!GMN::saveToFile(savePath, serializedCharacters)) {
        emit savingFailed(tr("Unable to save characters."));
    }
}

void Campaign::saveLocationsToFile(const LocationModelTypePtr &locationsModel)
{
    QString serializedLocations = locationsModel ? locationsModel->toJsonString() : GMNObjectModel<Location>().toJsonString();
    QString savePath(path + "/" + GMN::locationsFileName);
    if (!GMN::saveToFile(savePath, serializedLocations)) {
        emit savingFailed(tr("Unable to save locations."));
    }
}

CharacterModelTypePtr Campaign::createCharactersModel() const
{
    auto model = std::make_unique<GMNObjectModel<Character>>();
    std::optional<QString> charactersFile = GMN::loadJsonFile(path + "/" + GMN::charactersFileName);

    if (charactersFile) {
        model->loadFromJson(charactersFile.value().toUtf8());
    }

    return model;
}

LocationModelTypePtr Campaign::createLocationsModel() const
{
    auto model = std::make_unique<GMNObjectModel<Location>>();
    std::optional<QString> locationFile = GMN::loadJsonFile(path + "/" + GMN::locationsFileName);

    if (locationFile) {
        model->loadFromJson(locationFile.value().toUtf8());
    }

    return model;
}

void Campaign::prepareCharactersModel(GMNObjectModel<Character> &model)
{
    std::optional<QString> charactersFile = GMN::loadJsonFile(path + "/" + GMN::charactersFileName);

    if (charactersFile) {
        model.loadFromJson(charactersFile.value().toUtf8());
    }
}

void Campaign::prepareLocationsModel(GMNObjectModel<Location> &model)
{
    std::optional<QString> locationFile = GMN::loadJsonFile(path + "/" + GMN::locationsFileName);

    if (locationFile) {
        model.loadFromJson(locationFile.value().toUtf8());
    }
}

QString Campaign::getPath() const
{
    return path;
}

QDir Campaign::getDir() const
{
    return QDir(getPath());
}

void Campaign::clear()
{
    campaignInfo = CampaignInfo();
    path = QString();
}
