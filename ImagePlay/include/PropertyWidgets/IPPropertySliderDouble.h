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

#ifndef IPPROPERTYSLIDERDOUBLE_H
#define IPPROPERTYSLIDERDOUBLE_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertySliderDouble : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertySliderDouble(IPLProcessPropertyDouble* property, QWidget *parent) : IPPropertyWidget(property, parent)
    {
        setLayout(new QHBoxLayout);
        layout()->setMargin(0);

        _property = property;

        _slider = new QSlider(Qt::Horizontal, this);
        _spinner = new QDoubleSpinBox(this);

        layout()->addWidget(_slider);
        layout()->addWidget(_spinner);

        init();

        connect(_slider, &QSlider::valueChanged, this, &IPPropertySliderDouble::updateSpinner );
        connect(_spinner, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &IPPropertySliderDouble::updateSlider );

    }

    void init()
    {
        double min   = _property->min();
        double max   = _property->max();
        double value = _property->value();

        _slider->setMinimum(min*100);
        _slider->setMaximum(max*100);
        _slider->setValue(value*100);
        _slider->setTracking(true);

        _spinner->setMinimum(min);
        _spinner->setMaximum(max);
        _spinner->setValue(value);
        _spinner->setSingleStep(0.01);
    }

    void setMinimum(double v)  { _slider->setMinimum(v*100); }
    void setMaximum(double v)  { _slider->setMaximum(v*100); }
    double value()             { return _slider->value()/100.0; }

    void saveValue()        { _property->setValue(value()); }
    void resetValue()
    {
        _property->resetValue();
        init();
    }

signals:

public slots:
    void updateSpinner(int)
    {
        _spinner->setValue(value());

        updateValue();
    }

    void updateSlider(double v)
    {
        _slider->setValue(v*100);
    }


    void updateValue()
    {
        // prevent duplicate changes
        if(value() == _lastValue)
        {
            return;
        }

        if(_property)
            saveValue();

        emit changed();

        _lastValue = value();
    }

private:
    IPLProcessPropertyDouble* _property;
    QSlider*                    _slider;
    QDoubleSpinBox*             _spinner;
    double                      _lastValue;

};

#endif // IPPROPERTYSLIDERDOUBLE_H
