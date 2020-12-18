#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include "gmnobjectmodel.h"
#include "campaign.h"

#include <QObject>
#include <QStringListModel>

class ViewController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool campaignLoaded READ isCampaignLoaded NOTIFY campaignLoaded)
public:
    explicit ViewController(QObject *parent = nullptr);
    Q_INVOKABLE int getTestInt();
    Q_INVOKABLE QStringListModel* getCampaignsList();

    bool isCampaignLoaded() const;

public slots:
    bool loadCampaign(const QString& campaignName);
    bool createCampaign(const QString& campaignName);


signals:
    void campaignLoaded();
    void errorOccured(const QString& title, const QString& message);

private:
    Campaign campaign;
    GMNObjectModel<Character> charactersModel;
    GMNObjectModel<Location> locationsModel;
    bool m_campaignLoaded = false;

};

#endif // VIEWCONTROLLER_H
