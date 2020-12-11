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
    NOTES,
    IMAGEPATH
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

    QString getImagePath() const;
    void setImagePath(const QString &value);

    static constexpr int elementsCount() {return 8;}
    static constexpr int elementToInt(const CharacterElement element);

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

    QString imagePath;
    const QString imagePathKey = "image";
};

constexpr int Character::elementToInt(const CharacterElement element)
{
   switch (element) {
   case CharacterElement::AGE:
       return static_cast<int>(CharacterElement::AGE);

   case CharacterElement::NAME:
       return static_cast<int>(CharacterElement::NAME);

   case CharacterElement::BACKSTORY:
       return static_cast<int>(CharacterElement::BACKSTORY);

   case CharacterElement::DESCRIPTION:
       return static_cast<int>(CharacterElement::DESCRIPTION);

   case CharacterElement::NOTES:
       return static_cast<int>(CharacterElement::NOTES);

   case CharacterElement::PROFESSION:
       return static_cast<int>(CharacterElement::PROFESSION);

   case CharacterElement::RACE:
       return static_cast<int>(CharacterElement::RACE);

   case CharacterElement::IMAGEPATH:
       return static_cast<int>(CharacterElement::IMAGEPATH);
   default:

       return -1;
   }
}

#endif // CHARACTER_H
