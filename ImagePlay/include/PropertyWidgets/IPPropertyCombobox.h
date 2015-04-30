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

#ifndef IPPROPERTYCOMBOBOX_H
#define IPPROPERTYCOMBOBOX_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QComboBox>

#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertyCombobox : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyCombobox(IPLProcessPropertyInt* property, QWidget *parent) : IPPropertyWidget(parent)
    {
        _property = property;

        int value = property->value();

        setLayout(new QVBoxLayout(this));

        _combobox = new QComboBox(this);
        layout()->addWidget(_combobox);
        layout()->setMargin(0);

        // split the properties
        QString rawName(property->name());
        QString name = rawName.split(":").at(0);
        QString rawOptions = rawName.split(":").at(1);
        QStringList options = rawOptions.split("|");

        _combobox->addItems(options);

        _combobox->setCurrentIndex(value);

        connect(_combobox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &IPPropertyCombobox::valueChanged);
    }
    void setMinimum(int)  {  }
    void setMaximum(int)  {  }
    int value()             { return _combobox->currentIndex(); }

    void saveValue()        { _property->setValue(value()); }

signals:

public slots:
    void valueChanged()
    {
        saveValue();

        emit changed();
    }

private:
    IPLProcessPropertyInt*  _property;
    QComboBox*              _combobox;
};

#endif // IPPROPERTYCOMBOBOX_H
