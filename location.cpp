#include "location.h"

Location::Location(const Location &other)
{
    name = other.getName();
    setNotes(other.getNotes());
}

Location::Location(Location &&other)
    : name(std::exchange(other.name, nullptr)),
      notes(std::exchange(other.notes, nullptr))
{

}

Location &Location::operator=(const Location &other)
{
    return *this = Location(other);
}

Location &Location::operator=(Location &&other)
{
    std::swap(name, other.name);
    std::swap(notes, other.notes);
    return *this;
}

bool Location::setProperty(const int property, const QVariant &value)
{
    auto element = static_cast<LocationProperty>(property);

    switch (element) {
    case LocationProperty::NAME:
        setName(value.toString());
        break;

    case LocationProperty::NOTES:
        setNotes(value.toString());
        break;

    default:
        return false;
    }

    return true;
}

QVariant Location::getProperty(const int property) const
{
    auto element = static_cast<LocationProperty>(property);

    switch (element) {

    case LocationProperty::NAME:
        return QVariant(getName());

    case LocationProperty::NOTES:
        return QVariant(getNotes());

    default:
        return {};
    }
}

QVariant Location::getHeaderData(const int property) const
{
    auto propertyId = static_cast<LocationProperty>(property);

    switch (propertyId) {
    case LocationProperty::NAME:
      return QObject::tr("Name");


    case LocationProperty::NOTES:
       return QObject::tr("Notes");

    default:
        return {};
    }
}

bool Location::loadFromJsonDocument(const QJsonDocument &doc)
{
    if (!loadQString<Location>(doc[nameKey], this, &Location::setName)) {
        return false;
    }
    loadQString<Location>(doc[notesKey], this, &Location::setNotes);
    return true;
}

QJsonValue Location::serialize() const
{
    QJsonObject serialized {
        {nameKey, getName()},
        {notesKey, getNotes()}
    };

    return serialized;
}

QString Location::getName() const
{
    return name;
}

void Location::setName(const QString &value)
{
    name = value;
}

QString Location::getNotes() const
{
    return notes;
}

void Location::setNotes(const QString &value)
{
    notes = value;
}
