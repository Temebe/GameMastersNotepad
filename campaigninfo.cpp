#include "campaigninfo.h"

#include <QFile>
#include <QJsonObject>

CampaignInfo::CampaignInfo(const QString &path)
{
    QFile file(path);
    bool openResult = file.open(QFile::ReadOnly | QFile::Text);
    if (!openResult) {
        throw std::runtime_error("Unable to open file " + path.toStdString());
    }

    auto fileContent = file.readAll();
    file.close();

    if (!loadFromJson(fileContent)) {
        throw std::runtime_error("Your information file is corrupted: " + path.toStdString());
    }
}

bool CampaignInfo::loadFromJsonDocument(const QJsonDocument &doc)
{
    return loadQString<CampaignInfo>(doc[nameKey], this, &CampaignInfo::setName)
            && loadQDateTime<CampaignInfo>(doc[creationDateKey], this, &CampaignInfo::setCreationDate)
            && loadQDateTime<CampaignInfo>(doc[lastOpenedDateKey], this, &CampaignInfo::setLastOpenedDate);
}

QJsonValue CampaignInfo::serialize() const
{
    QJsonObject serialized {
        {nameKey, getName()},
        {creationDateKey, getCreationDate().toString(Qt::ISODate)},
        {lastOpenedDateKey, getLastOpenedDate().toString(Qt::ISODate)}
    };

    return serialized;
}

QString CampaignInfo::getName() const
{
    return name;
}

void CampaignInfo::setName(const QString &value)
{
    name = value;
}

QDateTime CampaignInfo::getCreationDate() const
{
    return creationDate;
}

void CampaignInfo::setCreationDate(const QDateTime &value)
{
    creationDate = value;
}

QDateTime CampaignInfo::getLastOpenedDate() const
{
    return lastOpenedDate;
}

void CampaignInfo::setLastOpenedDate(const QDateTime &value)
{
    lastOpenedDate = value;
}
