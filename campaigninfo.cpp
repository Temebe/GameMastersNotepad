#include "campaigninfo.h"

#include <QFile>
#include <QJsonObject>

CampaignInfo::CampaignInfo()
{

}

CampaignInfo::CampaignInfo(const QString &path)
{
    QFile file(path);
    bool openResult = file.open(QFile::ReadOnly | QFile::Text);
    if (!openResult) {
        throw std::runtime_error("Unable to open file " + path.toStdString());
    }

    auto fileContent = file.readAll();
    file.close();

    auto jsonDoc = QJsonDocument::fromJson(fileContent);
    if (!loadFromJson(jsonDoc)) {
        throw std::runtime_error("Your information file is corrupted: " + path.toStdString());
    }
}

bool CampaignInfo::loadFromJson(const QJsonDocument &doc)
{
    auto nameJson = doc[nameKey];
    if (nameJson == QJsonValue::Undefined) {
        return false;
    }
    setName(nameJson.toString());

    auto creationDateJson = doc[creationDateKey];
    if (creationDateJson == QJsonValue::Undefined) {
        return false;
    }
    auto creationDate = QDateTime::fromString(creationDateJson.toString(), Qt::ISODate);
    if (!creationDate.isValid()) {
        return false;
    }
    setCreationDate(creationDate);

    auto lastOpenedDateJson = doc[lastOpenedDateKey];
    if (lastOpenedDateJson == QJsonValue::Undefined) {
        return false;
    }
    auto lastOpenedDate = QDateTime::fromString(lastOpenedDateJson.toString(), Qt::ISODate);
    if (!lastOpenedDate.isValid()) {
        return false;
    }
    setLastOpenedDate(lastOpenedDate);

    return true;
}

QString CampaignInfo::serialize()
{
    QJsonObject serialized;

    serialized[nameKey] = getName();
    serialized[creationDateKey] = getCreationDate().toString(Qt::ISODate);
    serialized[lastOpenedDateKey] = getLastOpenedDate().toString(Qt::ISODate);

    return QJsonDocument(serialized).toJson();
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
