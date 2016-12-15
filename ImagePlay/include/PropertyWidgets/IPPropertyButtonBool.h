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

#ifndef IPPROPERTYBUTTONBOOL_H
#define IPPROPERTYBUTTONBOOL_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertyButtonBool : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyButtonBool(IPLProcessPropertyBool* property, QWidget *parent) :
        IPPropertyWidget(property, parent),
        _button(new QPushButton(this)),
        _value(false),
        _property(property)
    {
        setLayout(new QHBoxLayout);
        layout()->setMargin(0);

        _button->setText(property->title());
        layout()->addWidget(_button);

        connect(_button, &QPushButton::clicked, this, &IPPropertyButtonBool::onClick );
    }

    bool value() const      { return _value; }
    void saveValue()        { _property->setValue(value()); }
    void resetValue()       { _property->resetValue(); }

    void click()
    {
        _value = true;
        saveValue();
        emitChanged();
    }

protected:
    void emitChanged() { emit changed(); }

private slots:
    void onClick() { click(); }

private:
    QPushButton* _button;
    bool         _value;             // true if pressed during the last cycle. is set to false once value is accessed

    IPLProcessPropertyBool* _property;
};


#endif // IPPROPERTYBUTTONBOOL_H
