#ifndef CHARACTER_H
#define CHARACTER_H

#include "gmnserializable.h"

#include <QJsonDocument>
#include <QString>

enum class CharacterElement {
    NAME = 0,
    AGE,
    RACE,
    PROFESSION,
    BACKSTORY,
    DESCRIPTION,
    NOTES
};

class Character : public GMNSerializable
{
public:
    Character() = default;
    ~Character() = default;
    Character(const Character& other);
    Character(Character&& other);
    Character& operator=(const Character& other);
    Character& operator=(Character&& other);

    bool loadFromJsonDocument(const QJsonDocument &doc);
    QJsonValue serialize() const;

    QString getName() const;
    void setName(const QString &value);

    QString getAge() const;
    void setAge(const QString &value);

    QString getRace() const;
    void setRace(const QString &value);

    QString getProfession() const;
    void setProfession(const QString &value);

    QString getBackstory() const;
    void setBackstory(const QString &value);

    QString getDescription() const;
    void setDescription(const QString &value);

    QString getNotes() const;
    void setNotes(const QString &value);

    static constexpr int elementsCount() {return 7;}

private:
    QString name;
    const QString nameKey = "name";

    QString age;
    const QString ageKey = "age";

    QString race;
    const QString raceKey = "race";

    QString profession;
    const QString professionKey = "profession";

    QString backstory;
    const QString backstoryKey = "backstory";

    QString description;
    const QString descriptionKey = "description";

    QString notes;
    const QString notesKey = "notes";
};

#endif // CHARACTER_H
