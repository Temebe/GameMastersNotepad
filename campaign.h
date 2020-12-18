#ifndef CAMPAIGN_H
#define CAMPAIGN_H

#include "campaigninfo.h"
#include "character.h"
#include "location.h"
#include "gmnobjectmodel.h"
//#include "gmnutils.h"

#include <QObject>

#include <memory>

using CharacterModelTypePtr = std::unique_ptr<GMNObjectModel<Character>>;
using LocationModelTypePtr = std::unique_ptr<GMNObjectModel<Location>>;

class Campaign : public QObject
{
    Q_OBJECT
public:
    Campaign(QObject *parent = nullptr);
    ~Campaign() = default;
    Campaign(const Campaign& other);
    Campaign(Campaign&& other);
    Campaign& operator=(const Campaign& other);
    Campaign& operator=(Campaign&& other);

    void createNewCampaign(const QString &name);
    void loadFromName(const QString &name);
    void loadFromPath(const QString &path);

    void saveToFile(const CharacterModelTypePtr& charactersModel = nullptr, const LocationModelTypePtr& locationsModel = nullptr);
    void saveInfoFile();
    void saveCharactersToFile(const CharacterModelTypePtr& charactersModel);
    void saveLocationsToFile(const LocationModelTypePtr& locationsModel);


    CharacterModelTypePtr createCharactersModel() const;
    LocationModelTypePtr createLocationsModel() const;
    void prepareCharactersModel(GMNObjectModel<Character>& model);
    void prepareLocationsModel(GMNObjectModel<Location>& model);

//    template<typename T>
//    void fillModel(const QString &modelFilePath, GMNObjectModel<T>& model) const {
//        std::optional<QString> jsonFile = GMN::loadJsonFile(modelFilePath);
//        if (jsonFile) {
//            model.loadFromJson(jsonFile.value().toUtf8());
//        }
//    }

    QString getPath() const;
    QDir getDir() const;

signals:
    void succesfullyLoaded();
    void loadingFailed(const QString& reason, const QString& details = QString());
    void savingFailed(const QString& reason, const QString& details = QString());

private:
    CampaignInfo campaignInfo;
    QString path;

    void clear();
};

#endif // CAMPAIGN_H
