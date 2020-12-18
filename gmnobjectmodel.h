#ifndef GMNOBJECTMODEL_H
#define GMNOBJECTMODEL_H

#include "gmnobject.h"
#include "gmnserializable.h"

#include <QAbstractTableModel>
#include <QObject>

#include <memory>
#include <type_traits>

// Would be great to put guard of some sort
template<typename T>
class GMNObjectModel : public QAbstractTableModel, public GMNSerializable
{
public:
    GMNObjectModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    QHash<int, QByteArray> roleNames() const;

    bool loadFromJsonDocument(const QJsonDocument &doc) override;
    QJsonValue serialize() const override;

    std::optional<T> getObject(const QModelIndex& index);
    QModelIndex addObject(const QString &name);

    bool isEmpty() const;
    size_t getSize() const;

private:
    std::vector<T> collection;
};

#endif // GMNOBJECTMODEL_H
