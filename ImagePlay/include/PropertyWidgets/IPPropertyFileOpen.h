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

#ifndef IPPROPERTYFILE_H
#define IPPROPERTYFILE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

#include "IPPropertyWidget.h"
#include "IPLProcessProperty.h"

class IPPropertyFileOpen : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyFileOpen(IPLProcessPropertyString* property, QString defaultDirectory, QWidget* parent) : IPPropertyWidget(parent)
    {
        setLayout(new QHBoxLayout);
        layout()->setMargin(0);

        std::string value = ((IPLProcessPropertyString*) property)->value();

        _defaultDirectory = defaultDirectory;

        _lineEdit = new QLineEdit(QString::fromStdString(value));
        _lineEdit->setReadOnly(true);

        _button = new QPushButton("...");
        _button->setMaximumWidth(30);

        layout()->addWidget(_lineEdit);
        layout()->addWidget(_button);

        connect(_button, &QPushButton::pressed,
                this, &IPPropertyFileOpen::onPressButton );

        _property = property;
    }
    QString value()         { return _lineEdit->text(); }
    void saveValue()        { _property->setValue(value().toStdString()); }

signals:

public slots:
    void onPressButton()
    {
        QString directory;
        if(_defaultDirectory.length() > 0 && _lineEdit->text().length() == 0)
            directory = _defaultDirectory;

        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), directory, "Images (*.bmp *.gif *.hdr *.jpg *.png *.psd *.tif *.cr2 *.arw)");

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
    QString                     _defaultDirectory;

};

#endif // IPPROPERTYFILE_H
