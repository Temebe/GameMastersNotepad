#include "viewcontroller.h"

#include "gmnpaths.h"
#include "gmnobjectmodel.tpp"

#include <QDirIterator>
#include <QPixmap>
#include <QStringList>

ViewController::ViewController(QObject *parent)
    : QObject(parent),
      m_charactersModel(std::make_unique<GMNObjectModel<Character>>(this)),
      m_locationsModel(std::make_unique<GMNObjectModel<Location>>(this))
{
    connect(&m_campaign, &Campaign::succesfullyLoaded, this, &ViewController::configureCampaignView);
    connect(&m_campaign, &Campaign::loadingFailed, this, &ViewController::errorOccured);
    connect(&m_campaign, &Campaign::savingFailed, this, &ViewController::errorOccured);
    connect(m_charactersModel.get(), &QAbstractItemModel::dataChanged, this, &ViewController::charactersUpdated);
}

ViewController::~ViewController()
{
    saveCampaign();
}

int ViewController::getTestInt()
{
    return 42;
}

QStringListModel* ViewController::getCampaignsList()
{
    QStringListModel* model;
    QStringList campaignsList;
    QDirIterator it(GMN::campaignsFolder);

    while(it.hasNext()) {
        auto path = it.next();
        auto infoPath = path + "/" + GMN::infoFileName;
        CampaignInfo campaignInfo;
        if (!campaignInfo.loadFromFile(infoPath)) {
            continue;
        }

        campaignsList.append(campaignInfo.getName());
    }

    model = new QStringListModel();
    model->setStringList(campaignsList);
    return model;
}

QString ViewController::createImagePath(const QString &relativePath)
{
    const QString defaultImagePath = "qrc" + GMN::defaultCharacterImagePath;
    QFileInfo imageFileInfo(m_campaign.getDir(), relativePath);
    auto pixmap = QPixmap(imageFileInfo.canonicalFilePath());

    if (pixmap.isNull()) {
        return defaultImagePath;
    }
    return "file:" + imageFileInfo.canonicalFilePath();
}

QString ViewController::getCampaignImagesPath()
{
    return m_campaign.getPath() + "/" + GMN::imagesFolderName;
}

bool ViewController::isCampaignLoaded() const
{
    return m_campaignLoaded;
}

void ViewController::loadCampaign(const QString &campaignName)
{
    m_campaign.loadFromName(campaignName);
}


void ViewController::createCampaign(const QString &campaignName)
{
    m_campaign.createNewCampaign(campaignName);
}

void ViewController::configureCampaignView()
{
    m_campaign.prepareCharactersModel(*m_charactersModel.get());
    m_campaign.prepareLocationsModel(*m_locationsModel.get());

    m_campaignLoaded = true;
    emit campaignLoaded();
    emit imagesPathChanged();
}

void ViewController::addCharacter()
{
    m_charactersModel->addObject("Unnamed character");
}

void ViewController::removeCharacter(int index)
{
    m_charactersModel->removeRow(index);
}

void ViewController::addLocation()
{
    m_locationsModel->addObject("Unnamed location");
}

void ViewController::removeLocation(int index)
{
    m_locationsModel->removeRow(index);
}

void ViewController::changeCharacterImage(const int index, QString imagePath)
{
    if (imagePath.startsWith("file:")) {
        imagePath.remove(0, QString("file:").size());
    }
    auto modelIndex = m_charactersModel->index(index, Character::elementToInt(CharacterProperty::IMAGEPATH));
    m_charactersModel->setData(modelIndex, m_campaign.getDir().relativeFilePath(imagePath));
}

void ViewController::saveCampaign()
{
    m_campaign.saveToFile(m_charactersModel, m_locationsModel);
}

GMNObjectModel<Location>* ViewController::getLocationsModel()
{
    return m_locationsModel.get();
}

GMNObjectModel<Character>* ViewController::getCharactersModel()
{
    return m_charactersModel.get();
}
