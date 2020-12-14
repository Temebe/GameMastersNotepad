#ifndef CAMPAIGN_H
#define CAMPAIGN_H

#include "campaigninfo.h"
#include "character.h"
#include "location.h"
#include "gmnobjectmodel.h"

#include <QAbstractItemView>
#include <QObject>

#include <memory>

class Campaign : public QObject
{
    using CharacterModelType = std::unique_ptr<GMNObjectModel<Character>>;
    using LocationModelType = std::unique_ptr<GMNObjectModel<Location>>;
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

    void saveToFile(const CharacterModelType& charactersModel = nullptr, const LocationModelType& locationsModel = nullptr);
    void saveInfoFile();
    void saveCharactersToFile(const CharacterModelType& charactersModel);
    void saveLocationsToFile(const LocationModelType& locationsModel);


    CharacterModelType createCharactersModel() const;
    LocationModelType createLocationsModel() const;
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
