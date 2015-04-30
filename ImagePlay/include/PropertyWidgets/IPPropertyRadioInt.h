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

#ifndef IPPROPERTYRADIOINT_H
#define IPPROPERTYRADIOINT_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>

#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertyRadioInt : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyRadioInt(IPLProcessPropertyInt* property, QWidget *parent) : IPPropertyWidget(parent)
    {
        _property = property;

        setLayout(new QVBoxLayout);
        layout()->setMargin(0);

        int value = property->value();

        _buttonGroup = new QButtonGroup(this);

        // split the properties
        QString rawName(property->name());
        QString name = rawName.split(":").at(0);
        QString rawOptions = rawName.split(":").at(1);
        QStringList options = rawOptions.split("|");

        for(int i=0; i<options.size(); i++)
        {
            QRadioButton* btn = new QRadioButton(options[i], this);
            btn->setChecked(i==value);
            _buttonGroup->addButton(btn, i);
            layout()->addWidget(btn);


            connect(btn, &QRadioButton::clicked, this, &IPPropertyRadioInt::valueChanged);
        }

    }
    void setMinimum(int)  {  }
    void setMaximum(int)  {  }
    int value()             { return _buttonGroup->checkedId(); }

    void saveValue()        { _property->setValue(value()); }

signals:

public slots:
    void valueChanged()
    {
        saveValue();

        emit changed();
    }

private:
    IPLProcessPropertyInt*    _property;
    QButtonGroup*               _buttonGroup;
};

#endif // IPPROPERTYRADIOINT_H
