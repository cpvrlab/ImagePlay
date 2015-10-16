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

#ifndef IPPROPERTYFILECREATE_H
#define IPPROPERTYFILECREATE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

#include "IPPropertyWidget.h"
#include "IPLProcessProperty.h"

class IPPropertyFileSave : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyFileSave(IPLProcessPropertyString* property, QWidget* parent) : IPPropertyWidget(property, parent)
    {
        setLayout(new QHBoxLayout);
        layout()->setMargin(0);

        _property = property;

        // split the properties
        QString rawName(property->title());
        _filetypeString = rawName.split(":").at(1);

        _lineEdit = new QLineEdit(this);
        _lineEdit->setReadOnly(true);

        _button = new QPushButton("...");
        _button->setMaximumWidth(30);

        layout()->addWidget(_lineEdit);
        layout()->addWidget(_button);

        connect(_button, &QPushButton::pressed,
                this, &IPPropertyFileSave::onPressButton );

        init();
    }

    void init()
    {
        std::string value = _property->value();

        _lineEdit->setText(QString::fromStdString(value));
    }

    QString value()         { return _lineEdit->text(); }
    void saveValue()        { _property->setValue(value().toStdString()); }
    void resetValue()       { _property->resetValue(); init(); }

signals:

public slots:
    void onPressButton()
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", _filetypeString);

        if(fileName.length() > 0)
        {
            _lineEdit->setText(fileName);
            saveValue();

            emit changed();
        }
    }

private:
    IPLProcessPropertyString*   _property;
    QLineEdit*                  _lineEdit;
    QPushButton*                _button;
    QString                     _filetypeString;
};

#endif // IPPROPERTYFILECREATE_H
