#include "character.h"

#include <functional>

Character::Character(const Character &other)
{
    setAge(other.getAge());
    setBackstory(other.getBackstory());
    setDescription(other.getDescription());
    setName(other.getName());
    setNotes(other.getNotes());
    setProfession(other.getProfession());
    setRace(other.getRace());
}

Character::Character(Character &&other)
    : name(std::exchange(other.name, nullptr)),
      age(std::exchange(other.age, nullptr)),
      race(std::exchange(other.race, nullptr)),
      profession(std::exchange(other.profession, nullptr)),
      backstory(std::exchange(other.backstory, nullptr)),
      description(std::exchange(other.description, nullptr)),
      notes(std::exchange(other.notes, nullptr))
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
    return *this;
}

bool Character::loadFromJsonDocument(const QJsonDocument &doc)
{
    return loadQString<Character>(doc[nameKey], this, &Character::setName)
            && loadQString<Character>(doc[ageKey], this, &Character::setAge)
            && loadQString<Character>(doc[raceKey], this, &Character::setRace)
            && loadQString<Character>(doc[professionKey], this, &Character::setProfession)
            && loadQString<Character>(doc[backstoryKey], this, &Character::setBackstory)
            && loadQString<Character>(doc[descriptionKey], this, &Character::setDescription)
            && loadQString<Character>(doc[notesKey], this, &Character::setNotes);
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
        {notesKey, getNotes()}
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
