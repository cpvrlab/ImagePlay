#ifndef IPPROPERTYPOINT_H
#define IPPROPERTYPOINT_H

#include <QWidget>
#include <QSlider>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QApplication>
#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertyPoint : public IPPropertyWidget, public IPLCoordinatePickHandler
{
    Q_OBJECT
public:
    IPPropertyPoint(IPLProcessPropertyPoint* property, QWidget *parent) : IPPropertyWidget(parent)
    {
        setLayout(new QHBoxLayout);
        layout()->setMargin(0);

        _property = property;


        _positionPickCursor = new QCursor(QPixmap(":/crosshair_cursor.png"), 15, 15);

        _btnPositionPicker = new QPushButton(this);
        _btnPositionPicker->setCheckable(true);
        _btnPositionPicker->setMaximumWidth(30);
        _btnPositionPicker->setIcon(QIcon(":/crosshair.png"));

        //IPLPoint min   = property->min();
        //IPLPoint max   = property->max();
        IPLPoint value = property->value();

        _spinnerX = new QSpinBox(this);
        _spinnerY = new QSpinBox(this);

        layout()->addWidget(_spinnerX);
        layout()->addWidget(_spinnerY);
        layout()->addWidget(_btnPositionPicker);


        _spinnerX->setMinimum(0);
        _spinnerX->setMaximum(1000);
        _spinnerX->setValue(value.x());

        _spinnerY->setMinimum(0);
        _spinnerY->setMaximum(1000);
        _spinnerY->setValue(value.y());

        connect(_spinnerX, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IPPropertyPoint::updateValue );
        connect(_spinnerY, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IPPropertyPoint::updateValue );

        connect(_btnPositionPicker, &QPushButton::toggled, this, &IPPropertyPoint::btnPickerTriggered);
    }
    IPLPoint value()             { return IPLPoint(_spinnerX->value(), _spinnerY->value()); }

    void saveValue()        { _property->setValue(value()); }

    void pickCoordinates(int x, int y)
    {
        if(_btnPositionPicker->isChecked())
        {
            _spinnerX->setValue(x);
            _spinnerY->setValue(y);
        }
    }

public slots:
    void updateValue(int)
    {
        if(_property)
            saveValue();

        emit changed();
    }


    void btnPickerTriggered(bool status)
    {
        if(status)
        {
            QApplication::restoreOverrideCursor();
            QApplication::setOverrideCursor(*_positionPickCursor);
            _btnPositionPicker->setStyleSheet("background-color: #00ff00;");
        }
        else
        {
            //QApplication::setOverrideCursor(Qt::ArrowCursor);
            QApplication::restoreOverrideCursor();
            _btnPositionPicker->setStyleSheet("");
        }
    }

private:
    IPLProcessPropertyPoint*    _property;
    QSpinBox*                   _spinnerX;
    QSpinBox*                   _spinnerY;
    QPushButton*                _btnPositionPicker;
    QCursor*                    _positionPickCursor;
};

#endif // IPPROPERTYPOINT_H
