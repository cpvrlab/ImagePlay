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

#ifndef IPPROPERTYCHECKBOXINT_H
#define IPPROPERTYCHECKBOXINT_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QCheckBox>

#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertyCheckboxInt : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyCheckboxInt(IPLProcessPropertyUnsignedInt* property, QWidget *parent) : IPPropertyWidget(property, parent)
    {
        _ignoreUpdates = true;

        _property = property;

        setLayout(new QVBoxLayout);
        layout()->setMargin(0);

        // split the properties
        QString rawName(property->title());

        // check if structure is Title:Option1|Option2
        if(!(rawName.contains(":") && rawName.contains("|")))
        {
            qWarning() << "IPPropertyRadioInt: Invalid title structure " << rawName;
            return;
        }

        QString name = rawName.split(":").at(0);
        QString rawOptions = rawName.split(":").at(1);
        QStringList options = rawOptions.split("|");

        for(int i=0; i<options.length(); i++)
        {
            QCheckBox* btn = new QCheckBox(options[i], this);
            layout()->addWidget(btn);

            _buttonList.push_back(btn);

            connect(btn, &QCheckBox::clicked, this, &IPPropertyCheckboxInt::valueChanged);
        }

        init();

        _ignoreUpdates = false;
    }

    void init()
    {
        unsigned int value = _property->value();

        int i = 0;
        for(QCheckBox* btn : _buttonList)
        {
            bool checked = isOptionTrue(value, i++);
            btn->setChecked(checked);
        }
    }

    void setMinimum(int)  {  }
    void setMaximum(int)  {  }

    unsigned int value()
    {
        unsigned int value = 0;
        for(int i=0; i<_buttonList.size(); i++)
        {
            bool checked = _buttonList[i]->isChecked();
            setOption(value, i, checked);
        }
        return value;
    }


    // wert setzen => OR
    // wert entfernen => XOR
    void setOption(unsigned int &value, int index, bool checked)
    {
        unsigned int mask = 1 << index;
        if(checked)
        {
            value |= mask;
        }
        else
        {
            value &= ~mask;
        }
    }

    bool isOptionTrue(unsigned int value, int index)
    {

        unsigned int mask = 1 << index;

        if(value & mask)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void saveValue()        { _property->setValue(value()); }
    void resetValue()       { _property->resetValue(); init(); }

signals:

public slots:
    void valueChanged()
    {
        if(_ignoreUpdates)
            return;

        saveValue();

        emit changed();
    }

private:
    IPLProcessPropertyUnsignedInt*      _property;
    QList<QCheckBox*>                   _buttonList;
    bool                                _ignoreUpdates;
};

#endif // IPPROPERTYCHECKBOXINT_H
