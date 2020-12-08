#ifndef CAMPAIGNINFO_H
#define CAMPAIGNINFO_H

#include "gmnserializable.h"

#include <QDateTime>
#include <QJsonDocument>
#include <QString>



class CampaignInfo : public GMNSerializable
{
public:
    CampaignInfo() = default;
    CampaignInfo(const QString &path);

    bool loadFromJsonDocument(const QJsonDocument &doc);
    QJsonValue serialize() const;

    QString getName() const;
    void setName(const QString &value);

    QDateTime getCreationDate() const;
    void setCreationDate(const QDateTime &value);

    QDateTime getLastOpenedDate() const;
    void setLastOpenedDate(const QDateTime &value);

private:
    const QString nameKey = "name";
    const QString creationDateKey = "creation";
    const QString lastOpenedDateKey = "last_opened";

    QString name;
    QDateTime creationDate;
    QDateTime lastOpenedDate;
};

#endif // CAMPAIGNINFO_H
