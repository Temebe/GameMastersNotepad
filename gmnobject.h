#ifndef GMNOBJECT_H
#define GMNOBJECT_H

#include <QVariant>


class GMNObject
{
public:
    GMNObject() = default;
    virtual ~GMNObject() = default;

    virtual bool setProperty(const int property, const QVariant &value) = 0;
    virtual QVariant getProperty(const int property) const = 0;
    virtual QVariant getHeaderData(const int property) const = 0;
    virtual void setName(const QString& name) = 0;
};

#endif // GMNOBJECT_H
