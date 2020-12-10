#ifndef CAMPAIGN_H
#define CAMPAIGN_H

#include "campaigninfo.h"
#include "character.h"
#include "charactersmodel.h"

#include <QAbstractItemView>
#include <QObject>

#include <memory>

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

    void saveToFile(const std::unique_ptr<CharactersModel>& charactersModel = nullptr);
    void saveInfoFile();
    void saveCharactersToFile(const std::unique_ptr<CharactersModel>& charactersModel);


    std::unique_ptr<CharactersModel> createCharactersModel();
signals:
    void succesfullyLoaded();
    void loadingFailed(const QString& reason);
    void savingFailed(const QString& reason);

private:
    CampaignInfo campaignInfo;
    QString path;

    void clear();
};

#endif // CAMPAIGN_H
