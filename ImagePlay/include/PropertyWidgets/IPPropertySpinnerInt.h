#ifndef IPPROPERTYSPINNERINT_H
#define IPPROPERTYSPINNERINT_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpinBox>

#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertySpinner : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertySpinner(QWidget *parent) : IPPropertyWidget(parent)
    {
        setLayout(new QHBoxLayout);
        layout()->setMargin(0);

        _spinner = new QSpinBox;

        layout()->addWidget(_spinner);
    }

protected:
    template<class T> void setMinimum(T v)  { _spinner->setMinimum(v); }
    template<class T> void setMaximum(T v)  { _spinner->setMaximum(v); }
    template<class T> void setValue  (T v)  { _spinner->setValue(v); }

    template<class T> T value() const       { return _spinner->value(); }

signals:

public slots:

protected:
    QSpinBox* _spinner;
};

template<class T>
class IPPropertySpinnerImpl : public IPPropertySpinner
{
public:
    typedef T Property;
    typedef decltype(std::declval<T>().value()) Value;

    IPPropertySpinnerImpl(Property* property, QWidget *parent) :
        IPPropertySpinner(parent)
    {
        setMinimum(property->min());
        setMaximum(property->max());
        setValue(property->value());
        _property = property;
    }

    void saveValue()        { _property->setValue(value<Value>()); }

private:
    Property* _property;
};

typedef IPPropertySpinnerImpl<IPLProcessPropertyInt> IPPropertySpinnerInt;
typedef IPPropertySpinnerImpl<IPLProcessPropertyUnsignedInt> IPPropertySpinnerUnsignedInt;

#endif // IPPROPERTYSPINNERINT_H
