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
    Q_PROPERTY(QAbstractItemModel* charactersModel READ getCharactersModel NOTIFY charactersUpdated)
    Q_PROPERTY(QAbstractItemModel* locationsModel READ getLocationsModel NOTIFY locationsUpdated)
    Q_PROPERTY(QString imagesPath READ getCampaignImagesPath NOTIFY imagesPathChanged)
public:
    explicit ViewController(QObject *parent = nullptr);
    ~ViewController();
    Q_INVOKABLE int getTestInt();
    Q_INVOKABLE QStringListModel* getCampaignsList();
    Q_INVOKABLE QString createImagePath(const QString& relativePath);
    QString getCampaignImagesPath();
    GMNObjectModel<Character>* getCharactersModel();
    GMNObjectModel<Location>* getLocationsModel();

    bool isCampaignLoaded() const;



public slots:
    void loadCampaign(const QString& campaignName);
    void createCampaign(const QString& campaignName);
    void configureCampaignView();
    void addCharacter();
    void removeCharacter(const int index);
    void addLocation();
    void removeLocation(const int index);
    void changeCharacterImage(const int index, QString imagePath);
    void saveCampaign();


signals:
    void campaignLoaded();
    void errorOccured(const QString& title, const QString& message);
    void charactersUpdated();
    void locationsUpdated();
    void imagesPathChanged();

private:
    Campaign m_campaign;
    CharacterModelTypePtr m_charactersModel;
    LocationModelTypePtr m_locationsModel;
    bool m_campaignLoaded = false;

};

#endif // VIEWCONTROLLER_H
