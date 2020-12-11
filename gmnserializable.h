#ifndef GMNSERIALIZABLE_H
#define GMNSERIALIZABLE_H

#include <functional>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QDateTime>
#include <QDir>

class GMNSerializable
{
public:
    GMNSerializable() = default;
    virtual ~GMNSerializable() = default;

    virtual bool loadFromJsonDocument(const QJsonDocument &doc) = 0;
    virtual bool loadFromJson(const QString &jsonString) {
        auto jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
        if (jsonDoc.isNull()) {
            return false;
        }

        return loadFromJsonDocument(jsonDoc);
    }
    virtual QJsonValue serialize() const = 0;
    virtual QString toJsonString() const {
        auto jsonValue = serialize();
        if (jsonValue.isArray()) {
            return QJsonDocument(jsonValue.toArray()).toJson();
        }
        return QJsonDocument(jsonValue.toObject()).toJson();
    }

protected:
    template<typename T>
    bool loadQString(const QJsonValue value, T* object, std::function<void (T*, const QString&)> setterFunction) const {
        if (value.isUndefined()) {
            return false;
        }

        setterFunction(object, value.toString());
        return true;
    }

    template<typename T>
    bool loadQDateTime(const QJsonValue value, T* object, std::function<void (T*, const QDateTime&)> setterFunction) const {
        if (value.isUndefined()) {
            return false;
        }

        auto creationDate = QDateTime::fromString(value.toString(), Qt::ISODate);
        if (!creationDate.isValid()) {
            return false;
        }

        setterFunction(object, creationDate);
        return true;
    }
};

#endif // GMNSERIALIZABLE_H
