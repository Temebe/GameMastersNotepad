#include "character.h"

#include <QString>

#include <functional>

Character::Character(const Character &other)
{
    setAge(other.getAge());
    setBackstory(other.getBackstory());
    setDescription(other.getDescription());
    name = other.getName();
    setNotes(other.getNotes());
    setProfession(other.getProfession());
    setRace(other.getRace());
    setImagePath(other.getImagePath());
}

Character::Character(Character &&other)
    : name(std::exchange(other.name, nullptr)),
      age(std::exchange(other.age, nullptr)),
      race(std::exchange(other.race, nullptr)),
      profession(std::exchange(other.profession, nullptr)),
      backstory(std::exchange(other.backstory, nullptr)),
      description(std::exchange(other.description, nullptr)),
      notes(std::exchange(other.notes, nullptr)),
      imagePath(std::exchange(other.imagePath, nullptr))
{

}

Character &Character::operator=(const Character &other)
{
    return *this = Character(other);
}

Character &Character::operator=(Character &&other)
{
    std::swap(name, other.name);
    std::swap(age, other.age);
    std::swap(race, other.race);
    std::swap(description, other.description);
    std::swap(backstory, other.backstory);
    std::swap(notes, other.notes);
    std::swap(profession, other.profession);
    std::swap(imagePath, other.imagePath);
    return *this;
}

bool Character::setProperty(const int property, const QVariant &value)
{
    auto element = static_cast<CharacterProperty>(property);

    switch (element) {
    case CharacterProperty::AGE:
        setAge(value.toString());
        break;

    case CharacterProperty::NAME:
        setName(value.toString());
        break;

    case CharacterProperty::BACKSTORY:
        setBackstory(value.toString());
        break;

    case CharacterProperty::DESCRIPTION:
        setDescription(value.toString());
        break;

    case CharacterProperty::NOTES:
        setNotes(value.toString());
        break;

    case CharacterProperty::PROFESSION:
        setProfession(value.toString());
        break;

    case CharacterProperty::RACE:
        setRace(value.toString());
        break;

    case CharacterProperty::IMAGEPATH:
        setImagePath(value.toString());
        break;

    default:
        return false;
    }

    return true;
}

QVariant Character::getProperty(const int property) const
{
    auto element = static_cast<CharacterProperty>(property);

    switch (element) {
    case CharacterProperty::AGE:
        return QVariant(getAge());

    case CharacterProperty::NAME:
        return QVariant(getName());

    case CharacterProperty::BACKSTORY:
        return QVariant(getBackstory());

    case CharacterProperty::DESCRIPTION:
        return QVariant(getDescription());

    case CharacterProperty::NOTES:
        return QVariant(getNotes());

    case CharacterProperty::PROFESSION:
        return QVariant(getProfession());

    case CharacterProperty::RACE:
        return QVariant(getRace());

    case CharacterProperty::IMAGEPATH:
        return QVariant(getImagePath());

    default:
        return {};
    }
}

QVariant Character::getHeaderData(const int property) const
{
    auto propertyId = static_cast<CharacterProperty>(property);

    switch (propertyId) {
    case CharacterProperty::AGE:
       return QObject::tr("Age");

    case CharacterProperty::NAME:
      return QObject::tr("Name");

    case CharacterProperty::BACKSTORY:
      return QObject::tr("Backstory");

    case CharacterProperty::DESCRIPTION:
      return QObject::tr("Description");

    case CharacterProperty::NOTES:
       return QObject::tr("Notes");

    case CharacterProperty::PROFESSION:
      return QObject::tr("Profession");

    case CharacterProperty::RACE:
       return QObject::tr("Race");

    case CharacterProperty::IMAGEPATH:
      return QObject::tr("Image Path");

    default:
        return {};
    }
}

bool Character::loadFromJsonDocument(const QJsonDocument &doc)
{
    if (!loadQString<Character>(doc[nameKey], this, &Character::setName)) {
        return false;
    }
    loadQString<Character>(doc[ageKey], this, &Character::setAge);
    loadQString<Character>(doc[raceKey], this, &Character::setRace);
    loadQString<Character>(doc[professionKey], this, &Character::setProfession);
    loadQString<Character>(doc[backstoryKey], this, &Character::setBackstory);
    loadQString<Character>(doc[descriptionKey], this, &Character::setDescription);
    loadQString<Character>(doc[notesKey], this, &Character::setNotes);
    loadQString<Character>(doc[imagePathKey], this, &Character::setImagePath);
    return true;
}

QJsonValue Character::serialize() const
{
    QJsonObject serialized {
        {nameKey, getName()},
        {ageKey, getAge()},
        {raceKey, getRace()},
        {professionKey, getProfession()},
        {backstoryKey, getBackstory()},
        {descriptionKey, getDescription()},
        {notesKey, getNotes()},
        {imagePathKey, getImagePath()}
    };

    return serialized;
}

QString Character::getName() const
{
    return name;
}

void Character::setName(const QString &value)
{
    name = value;
}

QString Character::getAge() const
{
    return age;
}

void Character::setAge(const QString &value)
{
    age = value;
}

QString Character::getRace() const
{
    return race;
}

void Character::setRace(const QString &value)
{
    race = value;
}

QString Character::getProfession() const
{
    return profession;
}

void Character::setProfession(const QString &value)
{
    profession = value;
}

QString Character::getBackstory() const
{
    return backstory;
}

void Character::setBackstory(const QString &value)
{
    backstory = value;
}

QString Character::getDescription() const
{
    return description;
}

void Character::setDescription(const QString &value)
{
    description = value;
}

QString Character::getNotes() const
{
    return notes;
}

void Character::setNotes(const QString &value)
{
    notes = value;
}

QString Character::getImagePath() const
{
    return imagePath;
}

void Character::setImagePath(const QString &value)
{
    imagePath = value;
}
