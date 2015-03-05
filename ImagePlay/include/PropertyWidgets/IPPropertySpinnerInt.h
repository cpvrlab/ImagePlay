#ifndef IPPROPERTYSPINNERINT_H
#define IPPROPERTYSPINNERINT_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpinBox>

#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertySpinnerInt : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertySpinnerInt(IPLProcessPropertyInt* property, QWidget *parent) : IPPropertyWidget(parent)
    {
        setLayout(new QHBoxLayout);
        layout()->setMargin(0);

        int min   = ((IPLProcessPropertyInt*) property)->min();
        int max   = ((IPLProcessPropertyInt*) property)->max();
        int value = ((IPLProcessPropertyInt*) property)->value();

        _spinner = new QSpinBox;

        layout()->addWidget(_spinner);

        _spinner->setMinimum(min);
        _spinner->setMaximum(max);
        _spinner->setValue(value);

        _property = property;
    }
    void setMinimum(int v)  { _spinner->setMinimum(v); }
    void setMaximum(int v)  { _spinner->setMaximum(v); }
    int value()             { return _spinner->value(); }

    void saveValue()        { _property->setValue(value()); }

signals:

public slots:

private:
    IPLProcessPropertyInt* _property;
    QSpinBox* _spinner;
};

#endif // IPPROPERTYSPINNERINT_H
