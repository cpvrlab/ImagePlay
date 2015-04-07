#ifndef IPPROPERTYBUTTONUNSIGNEDINT_H
#define IPPROPERTYBUTTONUNSIGNEDINT_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertyButton : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyButton(IPLProcessProperty* property, QWidget *parent) :
        IPPropertyWidget(parent),
        _button(new QPushButton(this))
    {
        setLayout(new QHBoxLayout);
        layout()->setMargin(0);

        _button->setText(property->name());
        layout()->addWidget(_button);

        connect(_button, &QPushButton::clicked, this, &IPPropertyButton::onClick );
    }

    virtual void click() {}

protected:
    void emitChanged() { emit changed(); }

private slots:
    void onClick() { click(); }

private:
    QPushButton* _button;
};

template<class T>
class IPPropertyButtonImpl : public IPPropertyButton
{
public:
    typedef T Property;
    typedef decltype(std::declval<T>().value()) Value;

    IPPropertyButtonImpl(Property* property, QWidget *parent) :
        IPPropertyButton(property,parent),
        _property(property),
        _value(property->value())
    {}

    Value value() const { return _value; }
    void saveValue()        { _property->setValue(value()); }

    void click()
    {
        _value++;
        saveValue();
        emitChanged();
    }

private:
    int _value;

    Property* _property;
    QPushButton* _button;
};

typedef IPPropertyButtonImpl<IPLProcessPropertyInt> IPPropertyButtonInt;
typedef IPPropertyButtonImpl<IPLProcessPropertyUnsignedInt> IPPropertyButtonUnsignedInt;

#endif // IPPROPERTYBUTTONUNSIGNEDINT_H
