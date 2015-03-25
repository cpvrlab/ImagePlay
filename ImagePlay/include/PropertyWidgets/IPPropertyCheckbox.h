#ifndef IPPROPERTYCHECKBOX_H
#define IPPROPERTYCHECKBOX_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QCheckBox>

#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertyCheckbox : public IPPropertyWidget
{
    Q_OBJECT
public:
    explicit IPPropertyCheckbox(IPLProcessPropertyBool* property, QString title, QWidget *parent) : IPPropertyWidget(parent)
    {
        _property = property;

        _layout = new QGridLayout;
        _layout->setMargin(0);
        setLayout(_layout);

        bool value = ((IPLProcessPropertyBool*) property)->value();

        _checkbox = new QCheckBox(title, this);
        _checkbox->setChecked(value);

        // set up layout
        _layout->addWidget(_checkbox);

        // connect signals and slots
        connect(_checkbox, &QCheckBox::stateChanged, this, &IPPropertyCheckbox::updateValue );
    }
    void setMinimum(int)    {  }
    void setMaximum(int)    {  }
    bool value()            { return _checkbox->isChecked(); }

    void saveValue()        { _property->setValue(value()); }

signals:

public slots:
    void updateValue()
    {
        if(_property)
            saveValue();

        emit changed();
    }

private:
    IPLProcessPropertyBool* _property;
    QGridLayout* _layout;
    QCheckBox* _checkbox;
};

#endif // IPPROPERTYCHECKBOX_H
