#include "locationmodel.h"

LocationModel::LocationModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

int LocationModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return -1;
    }
    return static_cast<int>(locations.size());
}

int LocationModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return Location::elementsCount();
}

QVariant LocationModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return {};
    }

    auto row = static_cast<size_t>(index.row() >= 0 ? index.row() : std::numeric_limits<size_t>::max());
    if (row >= locations.size()) {
        return {};
    }

    auto location = locations.at(index.row());
    auto column = static_cast<LocationProperty>(index.column());

    switch (column) {
    case LocationProperty::NAME:
        return location.getName();

    case LocationProperty::NOTES:
        return location.getNotes();

    default:
        return {};
    }
}

QVariant LocationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical || role != Qt::DisplayRole) {
        return {};
    }

    auto column = static_cast<LocationProperty>(section);

    switch (column) {
    case LocationProperty::NAME:
        return tr("Name");
    case LocationProperty::NOTES:
        return tr("Notes");
    default:
        return {};
    }
}

bool LocationModel::setData(int row, const LocationProperty element, const QString &value)
{
    QModelIndex modelIndex;

    switch (element) {
    case LocationProperty::NAME:
    case LocationProperty::NOTES:
        modelIndex = index(row, Location::elementToInt(element));
        break;

    default:
        return false;
    }

    return setData(modelIndex, value, Qt::EditRole);
}

bool LocationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

}

Qt::ItemFlags LocationModel::flags(const QModelIndex &index) const
{

}

bool LocationModel::insertRows(int position, int rows, const QModelIndex &index)
{

}

bool LocationModel::removeRows(int position, int rows, const QModelIndex &index)
{

}

bool LocationModel::loadFromJsonDocument(const QJsonDocument &doc)
{

}

QJsonValue LocationModel::serialize() const
{

}



