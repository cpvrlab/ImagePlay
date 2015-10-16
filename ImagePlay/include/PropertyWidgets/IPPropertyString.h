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

#ifndef IPPROPERTYSTRING_H
#define IPPROPERTYSTRING_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

#include "IPPropertyWidget.h"
#include "IPLProcessProperty.h"

class IPPropertyString : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyString(IPLProcessPropertyString* property, QWidget* parent) : IPPropertyWidget(property, parent)
    {
        setLayout(new QHBoxLayout);
        layout()->setMargin(0);

        _property = property;

        _lineEdit = new QLineEdit(this);
        layout()->addWidget(_lineEdit);

        init();

        connect(_lineEdit, &QLineEdit::editingFinished, this, &IPPropertyString::onEditingFinished);

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
    void onEditingFinished()
    {
        saveValue();

        emit changed();
    }

private:
    IPLProcessPropertyString* _property;
    QLineEdit* _lineEdit;

};

#endif // IPPROPERTYSTRING_H
