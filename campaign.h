#ifndef CAMPAIGN_H
#define CAMPAIGN_H

#include "campaigninfo.h"
#include "character.h"
#include "characterslistmodel.h"

#include <QAbstractItemView>
#include <QObject>

class Campaign : public QObject
{
    Q_OBJECT
public:
    Campaign(QObject *parent = nullptr);
    ~Campaign() = default;
    void createNewCampaign(const QString &name);
    void loadFromName(const QString &name);
    void loadFromPath(const QString &path);
    void passCharactersModel(QAbstractItemView *itemView);

signals:
    void succesfullyLoaded();
    void loadingFailed(QString reason);

private:
    CampaignInfo campaignInfo;
    CharactersListModel characters;

    void clear();
};

#endif // CAMPAIGN_H
