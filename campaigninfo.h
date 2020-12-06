#ifndef CAMPAIGNINFO_H
#define CAMPAIGNINFO_H

#include <QDateTime>
#include <QJsonDocument>
#include <QString>



class CampaignInfo
{
public:
    CampaignInfo();
    CampaignInfo(const QString &path);

    bool loadFromJson(const QJsonDocument &doc);
    QString serialize();

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
