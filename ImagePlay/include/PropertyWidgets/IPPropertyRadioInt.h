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
    IPPropertyRadioInt(IPLProcessPropertyInt* property, QWidget *parent) : IPPropertyWidget(property, parent)
    {
        _ignoreEvents = true;

        _property = property;

        setLayout(new QVBoxLayout);
        layout()->setMargin(0);

        _buttonGroup = new QButtonGroup(this);

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

        for(int i=0; i<options.size(); i++)
        {
            QRadioButton* btn = new QRadioButton(options[i], this);
            _buttonGroup->addButton(btn, i);
            layout()->addWidget(btn);

            _inputs.push_back(btn);

            connect(btn, &QRadioButton::clicked, this, &IPPropertyRadioInt::valueChanged);
        }

        init();


        _ignoreEvents = false;
    }

    void init()
    {
        int value = _property->value();

        int i=0;
        for(QRadioButton* btn : _inputs)
        {
            btn->setChecked(i==value);
            i++;
        }
    }

    void setMinimum(int)  {  }
    void setMaximum(int)  {  }
    int value()             { return _buttonGroup->checkedId(); }

    void saveValue()        { _property->setValue(value()); }
    void resetValue()       { _property->resetValue(); init(); }

signals:

public slots:
    void valueChanged()
    {
        if(_ignoreEvents)
            return;

        saveValue();

        emit changed();
    }

private:
    IPLProcessPropertyInt*      _property;
    QButtonGroup*               _buttonGroup;
    QList<QRadioButton*>        _inputs;
    bool                        _ignoreEvents;
};

#endif // IPPROPERTYRADIOINT_H
