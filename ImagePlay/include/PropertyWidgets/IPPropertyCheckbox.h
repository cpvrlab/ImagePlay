//#############################################################################
//
//  This file is part of ImagePlay.
//
//  ImagePlay is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ImagePlay is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ImagePlay.  If not, see <http://www.gnu.org/licenses/>.
//
//#############################################################################

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
    explicit IPPropertyCheckbox(IPLProcessPropertyBool* property, QString title, QWidget *parent) : IPPropertyWidget(property, parent)
    {
        _property = property;

        _layout = new QGridLayout;
        _layout->setMargin(0);
        setLayout(_layout);

        _checkbox = new QCheckBox(title, this);

        // set up layout
        _layout->addWidget(_checkbox);

        init();

        // connect signals and slots
        connect(_checkbox, &QCheckBox::stateChanged, this, &IPPropertyCheckbox::updateValue );

    }

    void init()
    {
        bool value = ((IPLProcessPropertyBool*) _property)->value();
        _checkbox->setChecked(value);
    }

    void setMinimum(int)    {  }
    void setMaximum(int)    {  }
    bool value()            { return _checkbox->isChecked(); }

    void saveValue()        { _property->setValue(value()); }
    void resetValue()       { _property->resetValue(); init(); }

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
