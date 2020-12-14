#ifndef CHARACTER_H
#define CHARACTER_H

#include "gmnserializable.h"
#include "gmnobject.h"

#include <QJsonDocument>
#include <QString>

enum class CharacterProperty {
    NAME = 0,
    AGE,
    RACE,
    PROFESSION,
    BACKSTORY,
    DESCRIPTION,
    NOTES,
    IMAGEPATH
};

class Character : public GMNObject, public GMNSerializable
{
public:
    Character() = default;
    ~Character() = default;
    Character(const Character& other);
    Character(Character&& other);
    Character& operator=(const Character& other);
    Character& operator=(Character&& other);

    virtual bool setProperty(const int property, const QVariant &value) override;
    virtual QVariant getProperty(const int property) const override;
    virtual QVariant getHeaderData(const int property) const override;

    bool loadFromJsonDocument(const QJsonDocument &doc) override;
    QJsonValue serialize() const override;

    QString getName() const;
    void setName(const QString &value) override;

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

    static constexpr int elementsCount() { return 8; }
    static constexpr int elementToInt(const CharacterProperty element);
    using PropertyType = CharacterProperty;

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

constexpr int Character::elementToInt(const CharacterProperty element)
{
   switch (element) {
   case CharacterProperty::AGE:
   case CharacterProperty::NAME:
   case CharacterProperty::BACKSTORY:
   case CharacterProperty::DESCRIPTION:
   case CharacterProperty::NOTES:
   case CharacterProperty::PROFESSION:
   case CharacterProperty::RACE:
   case CharacterProperty::IMAGEPATH:
        return static_cast<int>(element);

   default:
       return -1;
   }
}

#endif // CHARACTER_H
