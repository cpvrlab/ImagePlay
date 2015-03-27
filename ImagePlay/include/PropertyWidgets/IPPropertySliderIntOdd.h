#ifndef IPPropertySliderIntOdd_H
#define IPPropertySliderIntOdd_H

#include <QWidget>
#include <QSlider>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertySliderIntOdd : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertySliderIntOdd(IPLProcessPropertyInt* property, QWidget *parent) : IPPropertyWidget(parent)
    {
        _ignoreEvents = true;
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
        _slider->setMaximum(max/2+1);
        _slider->setValue(value/2+1);

        _spinner->setMinimum(min);
        _spinner->setMaximum(max);
        _spinner->setValue(value);

        //connect(_slider, &QSlider::sliderReleased, this, &IPPropertySliderIntOdd::updateValue );
        connect(_slider, &QSlider::valueChanged, this, &IPPropertySliderIntOdd::onSliderChanged );
        connect(_spinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IPPropertySliderIntOdd::onSpinnerChanged );

        _ignoreEvents = false;

    }
    void setMinimum(int v)  { _slider->setMinimum(v);  _spinner->setMinimum(v); }
    void setMaximum(int v)  { _slider->setMaximum(v);  _spinner->setMaximum(v); }
    int value()             { return _spinner->value(); }

    void saveValue()        { _property->setValue(value()); }

signals:

public slots:
    void onSliderChanged(int v)
    {
        if(_ignoreEvents)
            return;
        qDebug() << "updateSpinner";
        _spinner->setValue(v*2-1);
    }

    void onSpinnerChanged(int v)
    {
        qDebug() << "updateSlider";

        // only allow odd values
        if(v%2 == 0)
        {
            _ignoreEvents = true;
            if(v > _lastValue)
                _spinner->setValue(v+1);
            else
                _spinner->setValue(v-1);
            _ignoreEvents = false;
            return;
        }

        _slider->setValue((v+1)/2);

        updateValue();
    }

    void updateValue()
    {
        qDebug() << "updateValue";

        int v = _spinner->value();

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
    bool    _ignoreEvents;

};

#endif // IPPropertySliderIntOdd_H
