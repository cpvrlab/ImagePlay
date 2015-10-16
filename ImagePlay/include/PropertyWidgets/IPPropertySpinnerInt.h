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

#ifndef IPPROPERTYSPINNERINT_H
#define IPPROPERTYSPINNERINT_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpinBox>

#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertySpinner : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertySpinner(IPLProcessProperty* property, QWidget *parent) : IPPropertyWidget(property, parent)
    {
        setLayout(new QHBoxLayout);
        layout()->setMargin(0);

        _spinner = new QSpinBox;

        layout()->addWidget(_spinner);
    }

protected:
    template<class T> void setMinimum(T v)  { _spinner->setMinimum(v); }
    template<class T> void setMaximum(T v)  { _spinner->setMaximum(v); }
    template<class T> void setValue  (T v)  { _spinner->setValue(v); }

    template<class T> T value() const       { return _spinner->value(); }

signals:

public slots:

protected:
    QSpinBox* _spinner;
};

template<class T>
class IPPropertySpinnerImpl : public IPPropertySpinner
{
public:
    typedef T Property;
    typedef decltype(std::declval<T>().value()) Value;

    IPPropertySpinnerImpl(Property* property, QWidget *parent) :
        IPPropertySpinner(property, parent)
    {
        setMinimum(property->min());
        setMaximum(property->max());
        setValue(property->value());
        _property = property;
    }

    void saveValue()        { _property->setValue(value<Value>()); }
    void resetValue()       { _property->resetValue(); }

private:
    Property* _property;
};

typedef IPPropertySpinnerImpl<IPLProcessPropertyInt> IPPropertySpinnerInt;
typedef IPPropertySpinnerImpl<IPLProcessPropertyUnsignedInt> IPPropertySpinnerUnsignedInt;

#endif // IPPROPERTYSPINNERINT_H
