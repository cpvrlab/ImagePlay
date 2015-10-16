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

#ifndef IPPROPERTYCOLORHSL_H
#define IPPROPERTYCOLORHSL_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpinBox>

#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertyColorHSL : public IPPropertyWidget
{
    Q_OBJECT
public:
    explicit IPPropertyColorHSL(IPLProcessPropertyColor* property, QWidget *parent) : IPPropertyWidget(property, parent)
    {
        _layout = new QGridLayout;
        _layout->setMargin(0);
        setLayout(_layout);

        _sliderH = new QSlider(Qt::Horizontal);
        _sliderS = new QSlider(Qt::Horizontal);
        _sliderL = new QSlider(Qt::Horizontal);

        _colorLabel = new QLabel("");
        _colorLabel->setMinimumHeight(20);
        _colorLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);


        QPalette paletteRed;
        paletteRed.setColor(QPalette::Highlight, QColor(0, 0, 0));
        _sliderH->setPalette(paletteRed);

        QPalette paletteGreen;
        paletteGreen.setColor(QPalette::Highlight, QColor(128, 128, 128));
        _sliderS->setPalette(paletteGreen);

        QPalette paletteBlue;
        paletteBlue.setColor(QPalette::Highlight, QColor(255, 255, 255));
        _sliderL->setPalette(paletteBlue);

        _spinH = new QSpinBox;
        _spinS = new QSpinBox;
        _spinL = new QSpinBox;

        _spinH->setMinimum(0);
        _spinH->setMaximum(359);
        _spinS->setMinimum(0);
        _spinS->setMaximum(255);
        _spinL->setMinimum(0);
        _spinL->setMaximum(255);

        _sliderH->setMinimum(0);
        _sliderH->setMaximum(359);
        _sliderS->setMinimum(0);
        _sliderS->setMaximum(255);
        _sliderL->setMinimum(0);
        _sliderL->setMaximum(255);

        _property = property;

        // set up layout
        _layout->addWidget(_colorLabel, 0, 2, 1, 1);
        _layout->addWidget(new QLabel("H"), 1, 0);
        _layout->addWidget(_sliderH, 1, 1);
        _layout->addWidget(_spinH, 1, 2);
        _layout->addWidget(new QLabel("S"), 2, 0);
        _layout->addWidget(_sliderS, 2, 1);
        _layout->addWidget(_spinS, 2, 2);
        _layout->addWidget(new QLabel("L"), 3, 0);
        _layout->addWidget(_sliderL, 3, 1);
        _layout->addWidget(_spinL, 3, 2);

        init();

        // connect signals and slots
        connect(_sliderH, &QSlider::valueChanged, _spinH, &QSpinBox::setValue );
        connect(_sliderS, &QSlider::valueChanged, _spinS, &QSpinBox::setValue );
        connect(_sliderL, &QSlider::valueChanged, _spinL, &QSpinBox::setValue );
        connect(_sliderH, &QSlider::valueChanged, this, &IPPropertyColorHSL::updateValue );
        connect(_sliderS, &QSlider::valueChanged, this, &IPPropertyColorHSL::updateValue );
        connect(_sliderL, &QSlider::valueChanged, this, &IPPropertyColorHSL::updateValue );

        // update everything
        updateValue();
    }

    void init()
    {
        IPLColor value = _property->value();

        _spinH->setValue(value.hue() * 359);
        _spinS->setValue(value.saturation() * 255);
        _spinL->setValue(value.lightness() * 255);
        _sliderH->setValue(value.hue() * 359);
        _sliderS->setValue(value.saturation() * 255);
        _sliderL->setValue(value.lightness() * 255);
    }

    void setMinimum(int)  {  }
    void setMaximum(int)  {  }
    IPLColor value()     { return IPLColor(_sliderH->value()/359, _sliderS->value()/255, _sliderL->value()/255); }

    void saveValue()        { _property->setValue(value()); }
    void resetValue()       { _property->resetValue(); init(); }

signals:

public slots:
    void updateValue()
    {
        // The value of s, v, and a must all be in the range 0-255;
        // the value of h must be in the range 0-359.
        QColor color = QColor::fromHsl(_sliderH->value(), _sliderS->value(), _sliderL->value());

        if(_property)
            saveValue();

        emit changed();

        // CSS is the easiest way to dynamically change the background
        _colorLabel->setStyleSheet(QString("height: 20px; width: 20px; background: rgb(%1,%2,%3);").arg(color.red()).arg(color.green()).arg(color.blue()));
    }

private:
    IPLProcessPropertyColor* _property;
    QGridLayout* _layout;
    QLabel* _colorLabel;
    QSlider* _sliderH;
    QSlider* _sliderS;
    QSlider* _sliderL;
    QSpinBox* _spinH;
    QSpinBox* _spinS;
    QSpinBox* _spinL;
};

#endif // IPPROPERTYCOLORHSL_H
