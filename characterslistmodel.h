#ifndef CHARACTERSLISTMODEL_H
#define CHARACTERSLISTMODEL_H

#include "character.h"
#include "gmnserializable.h"

#include <QObject>
#include <QAbstractTableModel>

class CharactersListModel : public QAbstractTableModel, public GMNSerializable
{
    Q_OBJECT
public:
    CharactersListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

    virtual bool loadFromJsonDocument(const QJsonDocument &doc);
    virtual QJsonValue serialize() const;

    void clear();

private:
    std::vector<Character> characters;
};

#endif // CHARACTERSLISTMODEL_H
