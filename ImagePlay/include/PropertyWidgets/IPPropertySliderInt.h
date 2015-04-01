#ifndef IPPROPERTYSLIDERINT_H
#define IPPROPERTYSLIDERINT_H

#include <QWidget>
#include <QSlider>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertySliderInt : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertySliderInt(IPLProcessPropertyInt* property, QWidget *parent) : IPPropertyWidget(parent)
    {
        _lastValue = 0;

        setLayout(new QHBoxLayout);
        layout()->setMargin(0);

        _property = property;

        int min   = property->min();
        int max   = property->max();
        int value = property->value();

        _slider = new QSlider(Qt::Horizontal, this);
        _spinner = new QSpinBox(this);

        layout()->addWidget(_slider);
        layout()->addWidget(_spinner);


        _slider->setMinimum(min);
        _slider->setMaximum(max);
        _slider->setValue(value);
        _slider->setSingleStep(1);

        _spinner->setMinimum(min);
        _spinner->setMaximum(max);
        _spinner->setValue(value);

        //connect(_slider, &QSlider::sliderReleased, this, &IPPropertySliderInt::updateValue );
        connect(_slider, &QSlider::valueChanged, this, &IPPropertySliderInt::onSliderChanged );
        connect(_spinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IPPropertySliderInt::onSpinnerChanged);
    }
    void setMinimum(int v)  { _slider->setMinimum(v);  _spinner->setMinimum(v); }
    void setMaximum(int v)  { _slider->setMaximum(v);  _spinner->setMaximum(v); }
    int value()             { return _slider->value(); }

    void saveValue()        { _property->setValue(value()); }

signals:

public slots:
    void onSliderChanged(int v)
    {
        _spinner->setValue(v);

        updateValue();
    }

    void onSpinnerChanged(int v)
    {
        _slider->setValue(v);
    }

    void updateValue()
    {
        int v = _slider->value();

        // prevent double changes
        if(v == _lastValue)
        {
            return;
        }

        if(_property)
            saveValue();

        emit changed();

        _lastValue = v;
    }

private:
    IPLProcessPropertyInt* _property;
    QSlider* _slider;
    QSpinBox* _spinner;
    int _lastValue;

};

#endif // IPPROPERTYSLIDERINT_H
