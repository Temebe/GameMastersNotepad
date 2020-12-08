#include "characterslistmodel.h"

#include <limits>

CharactersListModel::CharactersListModel()
{

}

int CharactersListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return characters.size();
}

int CharactersListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return Character::elementsCount();
}

QVariant CharactersListModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)

    auto row = static_cast<size_t>(index.row() >= 0 ? index.row() : std::numeric_limits<size_t>::max());
    if (row > characters.size()) {
        return QVariant();
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
    default:
        return QVariant();
    }
}

QVariant CharactersListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(role)

    if (orientation == Qt::Vertical) {
        return QVariant();
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
    default:
        return QVariant();
    }
}

bool CharactersListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role)

    auto row = static_cast<size_t>(index.row() >= 0 ? index.row() : std::numeric_limits<size_t>::max());
    if (row > characters.size() or value.canConvert(QMetaType::QString)) {
        return false;
    }

    QString stringValue = value.toString();
    auto character = characters.at(index.row());
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
    default:
        return false;
    }

    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
    return true;
}

bool CharactersListModel::insertRows(int position, int rows, const QModelIndex &index)
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
bool CharactersListModel::removeRows(int position, int rows, const QModelIndex &index)
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
    auto end = characters.begin() + position + rows - 1;
    characters.erase(begin, end);

    endRemoveRows();
    return true;
}

bool CharactersListModel::loadFromJsonDocument(const QJsonDocument &doc)
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

QJsonValue CharactersListModel::serialize() const
{
    QJsonArray array;
    for (const auto& character : characters) {
        array.append(character.serialize());
    }

    return array;
}

Qt::ItemFlags CharactersListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

void CharactersListModel::clear()
{

}
