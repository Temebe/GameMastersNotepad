#ifndef GMNOBJECTMODEL_TPP
#define GMNOBJECTMODEL_TPP

#include "gmnobjectmodel.h"

template<typename T>
GMNObjectModel<T>::GMNObjectModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

template<typename T>
int GMNObjectModel<T>::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return collection.size();
}

template<typename T>
int GMNObjectModel<T>::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return T::elementsCount();
}

template<typename T>
QVariant GMNObjectModel<T>::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole) {
        return {};
    }

    auto row = static_cast<size_t>(index.row() >= 0 ? index.row() : std::numeric_limits<size_t>::max());
    if (row >= collection.size()) {
        return {};
    }

    const auto& object = collection.at(index.row());
    return object.getProperty(index.column());
}

template<typename T>
QVariant GMNObjectModel<T>::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical || role != Qt::DisplayRole) {
        return {};
    }

    return T().getHeaderData(section);
}

template<typename T>
bool GMNObjectModel<T>::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    auto row = static_cast<size_t>(index.row() >= 0 ? index.row() : std::numeric_limits<size_t>::max());
    if (row > collection.size()) {
        return false;
    }

    auto& object = collection.at(index.row());
    if (!object.setProperty(index.column(), value)) {
        return false;
    }

    emit dataChanged(index, index);
    return true;
}

template<typename T>
Qt::ItemFlags GMNObjectModel<T>::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

template<typename T>
bool GMNObjectModel<T>::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    auto unsignedPosition = static_cast<size_t>(position >= 0 ? position : std::numeric_limits<size_t>::max());
    if (unsignedPosition > collection.size()) {
        return false;
    }

    beginInsertRows(QModelIndex(), position, position + rows - 1);

    collection.insert(collection.begin() + position, rows, T());

    endInsertRows();
    return true;
}

template<typename T>
bool GMNObjectModel<T>::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    auto unsignedPosition = static_cast<size_t>(position >= 0 ? position : std::numeric_limits<size_t>::max());
    auto unsignedRows = static_cast<size_t>(rows >= 0 ? rows : std::numeric_limits<size_t>::max());
    if (unsignedPosition == std::numeric_limits<size_t>::max() || unsignedRows == std::numeric_limits<size_t>::max()) {
        return false;
    }
    if (unsignedPosition >= collection.size() || unsignedPosition + unsignedRows - 1 >= collection.size()) {
        return false;
    }

    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    auto begin = collection.begin() + position;
    auto end = collection.begin() + position + rows;
    collection.erase(begin, end);

    endRemoveRows();
    return true;
}

template<typename T>
QHash<int, QByteArray> GMNObjectModel<T>::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "modelData";
    return roles;
}

template<typename T>
bool GMNObjectModel<T>::loadFromJsonDocument(const QJsonDocument &doc) {
    if (!doc.isArray() || !collection.empty()) {
        return false;
    }

    std::vector<T> newCollection;
    auto array = doc.array();
    for (const auto& value : array) {
        if (!value.isObject()) {
            continue;
        }

        T newObject;
        auto doc = QJsonDocument(value.toObject());
        if (newObject.loadFromJsonDocument(doc)) {
            newCollection.push_back(newObject);
        }
    }

    if (newCollection.empty()) {
        return true;
    }

    beginInsertRows(QModelIndex(), 0, newCollection.size() - 1);

    collection = std::move(newCollection);

    endInsertRows();
    return true;
}

template<typename T>
QJsonValue GMNObjectModel<T>::serialize() const {
    QJsonArray array;
    for (const auto& object : collection) {
        array.append(object.serialize());
    }

    return array;
}

template<typename T>
std::optional<T> GMNObjectModel<T>::getObject(const QModelIndex& index) {
    auto position = static_cast<size_t>(index.row() >= 0 ? index.row() : std::numeric_limits<size_t>::max());
    if (position < 0 || position >= collection.size()) {
        return {};
    }

    return collection.at(position);
}

template<typename T>
QModelIndex GMNObjectModel<T>::addObject(const QString &name) {
    auto newRow = collection.size();
    insertRow(newRow);
    (--collection.end())->setName(name);
    return index(newRow, 0);
}

template<typename T>
bool GMNObjectModel<T>::isEmpty() const
{
    return collection.empty();
}

template<typename T>
size_t GMNObjectModel<T>::getSize() const
{
    return collection.size();
}

#endif // GMNOBJECTMODEL_TPP
