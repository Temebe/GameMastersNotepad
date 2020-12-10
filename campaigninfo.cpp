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

CampaignInfo::CampaignInfo(const CampaignInfo &other)
    : name(other.name),
      creationDate(other.creationDate),
      lastOpenedDate(other.lastOpenedDate)
{

}

CampaignInfo::CampaignInfo(CampaignInfo &&other)
    : name(std::exchange(other.name, nullptr)),
      creationDate(std::exchange(other.creationDate, QDateTime())),
      lastOpenedDate(std::exchange(other.lastOpenedDate, QDateTime()))
{

}

CampaignInfo &CampaignInfo::operator=(const CampaignInfo &other)
{
    return *this = CampaignInfo(other);
}

CampaignInfo &CampaignInfo::operator=(CampaignInfo &&other)
{
    std::swap(name, other.name);
    std::swap(creationDate, other.creationDate);
    std::swap(lastOpenedDate, other.lastOpenedDate);
    return *this;
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
