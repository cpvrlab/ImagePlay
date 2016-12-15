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

#ifndef IPPROPERTYCOLORRGB_H
#define IPPROPERTYCOLORRGB_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QApplication>
#include <QColorDialog>

#include <QDebug>

#include "IPPropertyWidget.h"

class IPPropertyColorRGB : public IPPropertyWidget, public IPLColorPickHandler
{
    Q_OBJECT
public:
    explicit IPPropertyColorRGB(IPLProcessPropertyColor* property, QWidget *parent, IPLColorPickProvider* provider) : IPPropertyWidget(property, parent)
    {
        _layout = new QGridLayout;
        _layout->setMargin(0);
        setLayout(_layout);

        _dialog = NULL;

        _property = property;
        _colorProvider = provider;

        _colorPickCursor = new QCursor(QPixmap(":/colorpicker_cursor.png"), 9, 21);

        _sliderR = new QSlider(Qt::Horizontal);
        _sliderG = new QSlider(Qt::Horizontal);
        _sliderB = new QSlider(Qt::Horizontal);

        _colorLabel = new QPushButton("");
        _colorLabel->setMinimumHeight(20);
        //_colorLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);


        QPalette paletteRed;
        paletteRed.setColor(QPalette::Highlight, QColor(255, 0, 0));
        _sliderR->setPalette(paletteRed);

        QPalette paletteGreen;
        paletteGreen.setColor(QPalette::Highlight, QColor(0, 255, 0));
        _sliderG->setPalette(paletteGreen);

        QPalette paletteBlue;
        paletteBlue.setColor(QPalette::Highlight, QColor(0, 0, 255));
        _sliderB->setPalette(paletteBlue);

        _spinR = new QSpinBox;
        _spinG = new QSpinBox;
        _spinB = new QSpinBox;

        _spinR->setMinimum(0);
        _spinR->setMaximum(255);
        _spinG->setMinimum(0);
        _spinG->setMaximum(255);
        _spinB->setMinimum(0);
        _spinB->setMaximum(255);

        _sliderR->setMinimum(0);
        _sliderR->setMaximum(255);
        _sliderG->setMinimum(0);
        _sliderG->setMaximum(255);
        _sliderB->setMinimum(0);
        _sliderB->setMaximum(255);

        _btnColorPicker = new QPushButton(this);
        _btnColorPicker->setCheckable(true);
        _btnColorPicker->setIcon(QIcon(":/colorpicker.png"));

        // set up layout
        _layout->addWidget(_colorLabel, 0, 1, 1, 1);
        _layout->addWidget(_btnColorPicker, 0, 2, 1, 1);
        _layout->addWidget(new QLabel("R"), 1, 0);
        _layout->addWidget(_sliderR, 1, 1);
        _layout->addWidget(_spinR, 1, 2);
        _layout->addWidget(new QLabel("G"), 2, 0);
        _layout->addWidget(_sliderG, 2, 1);
        _layout->addWidget(_spinG, 2, 2);
        _layout->addWidget(new QLabel("B"), 3, 0);
        _layout->addWidget(_sliderB, 3, 1);
        _layout->addWidget(_spinB, 3, 2);

        init();

        // connect signals and slots
        connect(_sliderR, &QSlider::valueChanged, _spinR, &QSpinBox::setValue );
        connect(_sliderG, &QSlider::valueChanged, _spinG, &QSpinBox::setValue );
        connect(_sliderB, &QSlider::valueChanged, _spinB, &QSpinBox::setValue );
        connect(_sliderR, &QSlider::valueChanged, this, &IPPropertyColorRGB::updateValue );
        connect(_sliderG, &QSlider::valueChanged, this, &IPPropertyColorRGB::updateValue );
        connect(_sliderB, &QSlider::valueChanged, this, &IPPropertyColorRGB::updateValue );

        connect(_btnColorPicker, &QPushButton::toggled, this, &IPPropertyColorRGB::btnColorPickerTriggered);
        connect(_colorLabel, &QPushButton::clicked, this, &IPPropertyColorRGB::showColorpicker );


        // update everything
        updateValue();
    }

    void init()
    {
        IPLColor value = _property->value();

        _spinR->setValue(value.red() * 255);
        _spinG->setValue(value.green() * 255);
        _spinB->setValue(value.green() * 255);
        _sliderR->setValue(value.red() * 255);
        _sliderG->setValue(value.green() * 255);
        _sliderB->setValue(value.blue() * 255);
    }

    IPLColor value()     { return IPLColor(_sliderR->value()/255.0, _sliderG->value()/255.0, _sliderB->value()/255.0); }

    void saveValue()        { _property->setValue(value()); }
    void resetValue()       { _property->resetValue(); init(); }

    void pickColor(IPLColor color)
    {
        if(_btnColorPicker->isChecked())
        {
            _sliderR->setValue(color.red()*255);
            _sliderG->setValue(color.green()*255);
            _sliderB->setValue(color.blue()*255);

            _spinR->setValue(color.red()*255);
            _spinG->setValue(color.green()*255);
            _spinB->setValue(color.blue()*255);
        }
    }

    void onColorSelected(QColor color)
    {
        onColorPickerChanged(color);
    }

    void finishPickingColor()
    {
        _btnColorPicker->setChecked(false);
    }

signals:

public slots:
    void updateValue()
    {
        //QColor color(_sliderR->value(), _sliderG->value(), _sliderB->value());

        if(_property)
            saveValue();

        emit changed();

        // CSS is the easiest way to dynamically change the background
        _colorLabel->setStyleSheet(QString("height: 12px; width: 12px; background: rgb(%1,%2,%3);").arg(_sliderR->value()).arg(_sliderG->value()).arg(_sliderB->value()));
    }

    void btnColorPickerTriggered(bool status)
    {
        enableColorPicker(status);
    }

    void enableColorPicker(bool status)
    {
        if(status)
        {
            _colorProvider->setColorPickHandler(this);
            QApplication::restoreOverrideCursor();
            QApplication::setOverrideCursor(*_colorPickCursor);
            _btnColorPicker->setStyleSheet("background-color: #00ff00;");
        }
        else
        {
            _colorProvider->clearColorPickHandler();
            //QApplication::setOverrideCursor(Qt::ArrowCursor);
            QApplication::restoreOverrideCursor();
            _btnColorPicker->setStyleSheet("");
        }
    }

    void showColorpicker()
    {
        QColor color = QColor(_sliderR->value(), _sliderG->value(), _sliderB->value());

        _dialog = new QColorDialog(color);
        _dialog->setOptions(QColorDialog::DontUseNativeDialog);
        connect(_dialog, &QColorDialog::currentColorChanged, this, &IPPropertyColorRGB::onColorPickerChanged);
        connect(_dialog, &QColorDialog::colorSelected, this, &IPPropertyColorRGB::onColorSelected);
        _dialog->show();
    }

    void onColorPickerChanged(QColor color)
    {
        _sliderR->setValue(color.red());
        _sliderG->setValue(color.green());
        _sliderB->setValue(color.blue());
    }

private:
    IPLProcessPropertyColor* _property;
    IPLColorPickProvider* _colorProvider;
    QGridLayout* _layout;
    QPushButton* _colorLabel;
    QSlider* _sliderR;
    QSlider* _sliderG;
    QSlider* _sliderB;
    QSpinBox* _spinR;
    QSpinBox* _spinG;
    QSpinBox* _spinB;
    QPushButton*    _btnColorPicker;
    QCursor* _colorPickCursor;
    QColorDialog* _dialog;
};

#endif // IPPROPERTYCOLORRGB_H
