#ifndef CHARACTERSLISTMODEL_H
#define CHARACTERSLISTMODEL_H

#include "character.h"
#include "gmnserializable.h"

#include <QObject>
#include <QAbstractTableModel>

class CharactersModel : public QAbstractTableModel, public GMNSerializable
{
    Q_OBJECT
public:
    CharactersModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(int row, const CharacterElement element, const QString& value);
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

    virtual bool loadFromJsonDocument(const QJsonDocument &doc) override;
    virtual QJsonValue serialize() const override;

    std::optional<Character> getCharacter(const QModelIndex& index);
    QModelIndex addCharacter(const QString &name);

    void clear();
    size_t getSize() const;
    bool isEmpty() const;

private:
    std::vector<Character> characters;
};

#endif // CHARACTERSLISTMODEL_H
