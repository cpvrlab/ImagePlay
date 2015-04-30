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

#ifndef IPPROPERTYCOLORHSV_H
#define IPPROPERTYCOLORHSV_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QColorDialog>

#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertyColorHSV : public IPPropertyWidget
{
    Q_OBJECT
public:
    explicit IPPropertyColorHSV(IPLProcessPropertyColor* property, QWidget *parent) : IPPropertyWidget(parent)
    {
        _layout = new QGridLayout;
        _layout->setMargin(0);
        setLayout(_layout);

        _dialog = NULL;

        IPLColor value = ((IPLProcessPropertyColor*) property)->value();

        _sliderH = new QSlider(Qt::Horizontal);
        _sliderS = new QSlider(Qt::Horizontal);
        _sliderV = new QSlider(Qt::Horizontal);

        _colorLabel = new QPushButton("");
        _colorLabel->setMinimumHeight(20);
        //_colorLabel->setStyle(QFrame::StyledPanel | QFrame::Sunken);
        //_colorLabel->setAutoFillBackground(true);
        //_colorLabel->setFlat(true);
        //_colorLabel->setStyle(QFrame::StyledPanel | QFrame::Sunken);


        QPalette paletteRed;
        paletteRed.setColor(QPalette::Highlight, QColor(0, 0, 0));
        _sliderH->setPalette(paletteRed);

        QPalette paletteGreen;
        paletteGreen.setColor(QPalette::Highlight, QColor(128, 128, 128));
        _sliderS->setPalette(paletteGreen);

        QPalette paletteBlue;
        paletteBlue.setColor(QPalette::Highlight, QColor(255, 255, 255));
        _sliderV->setPalette(paletteBlue);

        _spinH = new QSpinBox;
        _spinS = new QSpinBox;
        _spinV = new QSpinBox;

        _spinH->setMinimum(0);
        _spinH->setMaximum(359);
        _spinH->setValue(value.hue() * 359);
        _spinS->setMinimum(0);
        _spinS->setMaximum(255);
        _spinS->setValue(value.saturation() * 255);
        _spinV->setMinimum(0);
        _spinV->setMaximum(255);
        _spinV->setValue(value.value() * 255);

        _sliderH->setMinimum(0);
        _sliderH->setMaximum(359);
        _sliderH->setValue(value.hue() *359);
        _sliderS->setMinimum(0);
        _sliderS->setMaximum(255);
        _sliderS->setValue(value.saturation() * 255);
        _sliderV->setMinimum(0);
        _sliderV->setMaximum(255);
        _sliderV->setValue(value.value() * 255);

        _property = property;

        // set up layout
        _layout->addWidget(_colorLabel, 0, 2, 1, 1);
        _layout->addWidget(new QLabel("H"), 1, 0);
        _layout->addWidget(_sliderH, 1, 1);
        _layout->addWidget(_spinH, 1, 2);
        _layout->addWidget(new QLabel("S"), 2, 0);
        _layout->addWidget(_sliderS, 2, 1);
        _layout->addWidget(_spinS, 2, 2);
        _layout->addWidget(new QLabel("V"), 3, 0);
        _layout->addWidget(_sliderV, 3, 1);
        _layout->addWidget(_spinV, 3, 2);

        // connect signals and slots
        connect(_sliderH, &QSlider::valueChanged, _spinH, &QSpinBox::setValue );
        connect(_sliderS, &QSlider::valueChanged, _spinS, &QSpinBox::setValue );
        connect(_sliderV, &QSlider::valueChanged, _spinV, &QSpinBox::setValue );
        connect(_sliderH, &QSlider::valueChanged, this, &IPPropertyColorHSV::updateValue );
        connect(_sliderS, &QSlider::valueChanged, this, &IPPropertyColorHSV::updateValue );
        connect(_sliderV, &QSlider::valueChanged, this, &IPPropertyColorHSV::updateValue );


        connect(_colorLabel, &QPushButton::clicked, this, &IPPropertyColorHSV::showColorpicker );

        // update everything
        updateValue();
    }
    void setMinimum(int)  {  }
    void setMaximum(int)  {  }
    IPLColor value()     { return IPLColor(_sliderH->value()/359, _sliderS->value()/255, _sliderV->value()/255); }

    void saveValue()        { _property->setValue(value()); }

signals:

public slots:
    void updateValue()
    {
        // The value of s, v, and a must all be in the range 0-255;
        // the value of h must be in the range 0-359.
        QColor color = QColor::fromHsv(_sliderH->value(), _sliderS->value(), _sliderV->value());

        // CSS is the easiest way to dynamically change the background
        _colorLabel->setStyleSheet(QString("border: 1px solid rgba(128,128,128,50%); border-radius: 2px; height: 20px; width: 20px; background: rgb(%1,%2,%3);").arg(color.red()).arg(color.green()).arg(color.blue()));
    }

    void showColorpicker()
    {
        QColor color = QColor::fromHsv(_sliderH->value(), _sliderS->value(), _sliderV->value());

        qDebug() << "color: " << color;

        _dialog = new QColorDialog(color);
        _dialog->setOptions(QColorDialog::DontUseNativeDialog);
        connect(_dialog, &QColorDialog::currentColorChanged, this, &IPPropertyColorHSV::onColorPickerChanged);
        connect(_dialog, &QColorDialog::colorSelected, this, &IPPropertyColorHSV::onColorSelected);
        _dialog->show();
    }

    void onColorPickerChanged(QColor color)
    {
        _sliderH->setValue(color.hsvHue());
        _sliderS->setValue(color.hsvSaturation());
        _sliderV->setValue(color.value());
    }

    void onColorSelected(QColor color)
    {
        qDebug() << "onColorSelected";
        onColorPickerChanged(color);
    }

private:
    IPLProcessPropertyColor* _property;
    QGridLayout* _layout;
    QPushButton* _colorLabel;
    QSlider* _sliderH;
    QSlider* _sliderS;
    QSlider* _sliderV;
    QSpinBox* _spinH;
    QSpinBox* _spinS;
    QSpinBox* _spinV;
    QColorDialog* _dialog;
};

#endif // IPPROPERTYCOLORHSV_H
