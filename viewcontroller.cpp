#include "viewcontroller.h"

#include "gmnpaths.h"

#include <QDirIterator>
#include <QStringList>

ViewController::ViewController(QObject *parent) : QObject(parent)
{

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

bool ViewController::isCampaignLoaded() const
{
    return m_campaignLoaded;
}

bool ViewController::loadCampaign(const QString &campaignName)
{
    m_campaignLoaded = true;
    emit campaignLoaded();
    return true;
}


bool ViewController::createCampaign(const QString &campaignName)
{
    emit errorOccured("title", "message");
    //m_campaignLoaded = true;
    //emit campaignLoaded();
    return true;
}
