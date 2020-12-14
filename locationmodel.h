#ifndef LOCATIONMODEL_H
#define LOCATIONMODEL_H

#include "location.h"
#include "gmnserializable.h"

#include <QAbstractTableModel>
#include <QObject>

class LocationModel : public QAbstractTableModel, public GMNSerializable
{
    Q_OBJECT
public:
    LocationModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(int row, const LocationProperty element, const QString& value);
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

    virtual bool loadFromJsonDocument(const QJsonDocument &doc) override;
    virtual QJsonValue serialize() const override;

private:
    std::vector<Location> locations;
};

#endif // LOCATIONMODEL_H
