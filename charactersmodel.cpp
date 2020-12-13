#include "charactersmodel.h"

#include <limits>

CharactersModel::CharactersModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

int CharactersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return characters.size();
}

int CharactersModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return Character::elementsCount();
}

QVariant CharactersModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return {};
    }

    auto row = static_cast<size_t>(index.row() >= 0 ? index.row() : std::numeric_limits<size_t>::max());
    if (row >= characters.size()) {
        return {};
    }

    auto character = characters.at(index.row());
    auto column = static_cast<CharacterElement>(index.column());

    switch (column) {
    case CharacterElement::AGE:
        return character.getAge();
    case CharacterElement::NAME:
        return character.getName();
    case CharacterElement::BACKSTORY:
        return character.getBackstory();
    case CharacterElement::DESCRIPTION:
        return character.getDescription();
    case CharacterElement::NOTES:
        return character.getNotes();
    case CharacterElement::PROFESSION:
        return character.getProfession();
    case CharacterElement::RACE:
        return character.getRace();
    case CharacterElement::IMAGEPATH:
        return character.getImagePath();
    default:
        return {};
    }
}

QVariant CharactersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical || role != Qt::DisplayRole) {
        return {};
    }

    auto column = static_cast<CharacterElement>(section);

    switch (column) {
    case CharacterElement::AGE:
        return tr("Age");
    case CharacterElement::NAME:
        return tr("Name");
    case CharacterElement::BACKSTORY:
        return tr("Backstory");
    case CharacterElement::DESCRIPTION:
        return tr("Description");
    case CharacterElement::NOTES:
        return tr("Notes");
    case CharacterElement::PROFESSION:
        return tr("Profession");
    case CharacterElement::RACE:
        return tr("Race");
    case CharacterElement::IMAGEPATH:
        return tr("Image Path");
    default:
        return {};
    }
}

bool CharactersModel::setData(int row, const CharacterElement element, const QString &value)
{
    QModelIndex modelIndex;

    switch (element) {
    case CharacterElement::AGE:
        modelIndex = index(row, Character::elementToInt(CharacterElement::AGE));
        break;

    case CharacterElement::NAME:
        modelIndex = index(row, Character::elementToInt(CharacterElement::NAME));
        break;

    case CharacterElement::BACKSTORY:
        modelIndex = index(row, Character::elementToInt(CharacterElement::BACKSTORY));
        break;

    case CharacterElement::DESCRIPTION:
        modelIndex = index(row, Character::elementToInt(CharacterElement::DESCRIPTION));
        break;

    case CharacterElement::NOTES:
        modelIndex = index(row, Character::elementToInt(CharacterElement::NOTES));
        break;

    case CharacterElement::PROFESSION:
        modelIndex = index(row, Character::elementToInt(CharacterElement::PROFESSION));
        break;

    case CharacterElement::RACE:
        modelIndex = index(row, Character::elementToInt(CharacterElement::RACE));
        break;

    case CharacterElement::IMAGEPATH:
        modelIndex = index(row, Character::elementToInt(CharacterElement::IMAGEPATH));
        break;

    default:
        return false;
    }

    return setData(modelIndex, value, Qt::EditRole);
}

bool CharactersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    auto row = static_cast<size_t>(index.row() >= 0 ? index.row() : std::numeric_limits<size_t>::max());
    if (row > characters.size() || !value.canConvert(QMetaType::QString)) {
        return false;
    }

    QString stringValue = value.toString();
    auto& character = characters.at(index.row());
    auto column = static_cast<CharacterElement>(index.column());

    switch (column) {
    case CharacterElement::AGE:
        character.setAge(stringValue);
        break;

    case CharacterElement::NAME:
        character.setName(stringValue);
        break;

    case CharacterElement::BACKSTORY:
        character.setBackstory(stringValue);
        break;

    case CharacterElement::DESCRIPTION:
        character.setDescription(stringValue);
        break;

    case CharacterElement::NOTES:
        character.setNotes(stringValue);
        break;

    case CharacterElement::PROFESSION:
        character.setProfession(stringValue);
        break;

    case CharacterElement::RACE:
        character.setRace(stringValue);
        break;

    case CharacterElement::IMAGEPATH:
        character.setImagePath(stringValue);
        break;

    default:
        return false;
    }

    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
    return true;
}

bool CharactersModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    auto unsignedPosition = static_cast<size_t>(position >= 0 ? position : std::numeric_limits<size_t>::max());
    if (unsignedPosition > characters.size()) {
        return false;
    }

    beginInsertRows(QModelIndex(), position, position + rows - 1);

    characters.insert(characters.begin() + position, rows, Character());

    endInsertRows();
    return true;
}

// TODO I bet there is simpler way
bool CharactersModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    auto unsignedPosition = static_cast<size_t>(position >= 0 ? position : std::numeric_limits<size_t>::max());
    auto unsignedRows = static_cast<size_t>(rows >= 0 ? rows : std::numeric_limits<size_t>::max());
    if (unsignedPosition == std::numeric_limits<size_t>::max() || unsignedRows == std::numeric_limits<size_t>::max()) {
        return false;
    }
    if (unsignedPosition >= characters.size() || unsignedPosition + unsignedRows - 1 >= characters.size()) {
        return false;
    }

    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    auto begin = characters.begin() + position;
    auto end = characters.begin() + position + rows;
    characters.erase(begin, end);

    endRemoveRows();
    return true;
}

bool CharactersModel::loadFromJsonDocument(const QJsonDocument &doc)
{
    if (!doc.isArray()) {
        return false;
    }

    auto array = doc.array();
    for (const auto& value : array) {
        if (!value.isObject()) {
            continue;
        }

        Character newCharacter;
        auto doc = QJsonDocument(value.toObject());
        if (newCharacter.loadFromJsonDocument(doc)) {
            characters.push_back(newCharacter);
        }
    }
    return true;
}

QJsonValue CharactersModel::serialize() const
{
    QJsonArray array;
    for (const auto& character : characters) {
        array.append(character.serialize());
    }

    return array;
}

std::optional<Character> CharactersModel::getCharacter(const QModelIndex &index)
{
    auto position = static_cast<size_t>(index.row() >= 0 ? index.row() : std::numeric_limits<size_t>::max());
    if (position >= characters.size()) {
        return {};
    }

    return characters.at(position);
}

QModelIndex CharactersModel::addCharacter(const QString &name)
{
    if (name.isEmpty()) {
        return QModelIndex();
    }
    auto newRow = characters.size();
    insertRow(newRow);
    (--characters.end())->setName(name);
    return index(newRow, 0);
}

Qt::ItemFlags CharactersModel::flags(const QModelIndex &index) const
{
    //return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

void CharactersModel::clear()
{

}

bool CharactersModel::isEmpty() const
{
    return characters.empty();
}

size_t CharactersModel::getSize() const
{
    return characters.size();
}
