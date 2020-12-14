#ifndef LOCATION_H
#define LOCATION_H

#include "gmnserializable.h"
#include "gmnobject.h"

enum class LocationProperty {
    NAME = 0,
    NOTES
};

class Location : public GMNObject, public GMNSerializable
{
public:
    Location() = default;
    ~Location() = default;
    Location(const Location& other);
    Location(Location &&other);
    Location& operator=(const Location& other);
    Location& operator=(Location&& other);

    virtual bool setProperty(const int property, const QVariant &value) override;
    virtual QVariant getProperty(const int property) const override;
    virtual QVariant getHeaderData(const int property) const override;

    bool loadFromJsonDocument(const QJsonDocument &doc) override;
    QJsonValue serialize() const override;

    QString getName() const;
    void setName(const QString &value) override;

    QString getNotes() const;
    void setNotes(const QString &value);

    static constexpr int elementsCount() { return 2; }
    static constexpr int elementToInt(const LocationProperty element);
    using ElementType = LocationProperty;

private:
    QString name;
    const QString nameKey = "name";

    QString notes;
    const QString notesKey = "notes";

};

constexpr int Location::elementToInt(const LocationProperty element)
{
    switch (element) {
    case LocationProperty::NAME:
    case LocationProperty::NOTES:
        return static_cast<int>(element);

    default:
        return -1;
    }
}

#endif // LOCATION_H
