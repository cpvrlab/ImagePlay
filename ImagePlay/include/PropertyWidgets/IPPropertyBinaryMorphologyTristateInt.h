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

#ifndef IPPROPERTYBINARYMORPHOLOGYTRISTATEINT_H
#define IPPROPERTYBINARYMORPHOLOGYTRISTATEINT_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QCheckBox>
#include <QComboBox>

#include <QDebug>

#include "IPPropertyWidget.h"

#include "../IPMorphologyCheckbox.h"

#include "IPKernelPreset.h"

class IPPropertyBinaryMorphologyTristateInt : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyBinaryMorphologyTristateInt(IPLProcessPropertyVectorInt* property, QWidget *parent) : IPPropertyWidget(property, parent)
    {
        _ignoreCombobox = true;
        _ignoreKernel = true;

        // init presets
        int presetEmpty[]       = {0,0,0,0,0,0,0,0,0};
        int presetDontCare[]    = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
        int presetTopLeft[]     = {0,0,-1,0,1,1,-1,1,-1};
        int presetTopRight[]    = {-1,0,0,1,1,0,-1,1,-1};
        int presetBottomLeft[]  = {-1,1,-1,0,1,1,0,0,-1};
        int presetBottomRight[] = {-1,1,-1,1,1,0,-1,0,0};

        // add to preset list
        _presets.push_back(IPKernelPreset("Custom",             presetEmpty, 9));
        _presets.push_back(IPKernelPreset("Don't care",         presetDontCare, 9));
        _presets.push_back(IPKernelPreset("Corner Top Left",    presetTopLeft, 9));
        _presets.push_back(IPKernelPreset("Corner Top Right",   presetTopRight, 9));
        _presets.push_back(IPKernelPreset("Corner Bottom Left", presetBottomLeft, 9));
        _presets.push_back(IPKernelPreset("Corner Bottom Right",presetBottomRight, 9));

        // get property values
        _property = property;
        _kernel = ((IPLProcessPropertyVectorInt*) property)->value();

        _kernelType = 0;        // 3x3
        if(_kernel.size() == 25)
        {
            _kernelType = 1;    // 5x5
        }
        else if(_kernel.size() == 49)
        {
            _kernelType = 2;    // 7x7
        }
        else if(_kernel.size() == 81)
        {
            _kernelType = 3;    // 9x9
        }

        setLayout(new QVBoxLayout(this));
        _gridLayout = new QGridLayout;
        _gridLayout->setSpacing(2);

        _kernelWidget = new QWidget(this);
        _kernelWidget->setLayout(_gridLayout);
        layout()->addWidget(_kernelWidget );

        _kernelSizeComboBox = new QComboBox(this);
        _kernelSizeComboBox->addItem("3x3");
        _kernelSizeComboBox->addItem("5x5");
        _kernelSizeComboBox->addItem("7x7");
        _kernelSizeComboBox->addItem("9x9");
        _kernelSizeComboBox->setCurrentIndex(_kernelType);
        connect(_kernelSizeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &IPPropertyBinaryMorphologyTristateInt::enableDisableKernelEditor);

        layout()->addWidget(_kernelSizeComboBox);

        int checkboxSize = 16;

        // compute offset
        int offset = 4-((int)sqrt((float)_kernel.size()) / 2);
        int j = 0;
        for(int i=0; i<81; i++)
        {
            int column = i%9;
            int row    = i/9;

            QCheckBox* input = new IPMorphologyCheckbox(this);
            input->setMinimumWidth(checkboxSize);
            input->setMaximumWidth(checkboxSize);
            input->setMinimumHeight(checkboxSize);
            input->setMaximumHeight(checkboxSize);

            input->setTristate(true);

            if(column >= offset && row >= offset && column < 9-offset && row < 9-offset)
            {
                int state = _kernel[j++];
                if(state == -1)
                    input->setCheckState(Qt::PartiallyChecked);
                else if(state == 1)
                    input->setCheckState(Qt::Checked);
                else
                    input->setCheckState(Qt::Unchecked);
            }
            else
            {
                input->setChecked(false);
            }

            _gridLayout->addWidget(input, i/9, i%9);
            _inputs.push_back(input);

            connect(input, &QCheckBox::stateChanged, this, &IPPropertyBinaryMorphologyTristateInt::updateValue );
        }

        _presetsComboBox = new QComboBox(this);
        for(size_t i=0; i<_presets.size(); i++)
        {
            _presetsComboBox->addItem(QString::fromStdString(_presets[i].name));
        }
        layout()->addWidget(_presetsComboBox);

        enableDisableKernelEditor();
        setKernelPreset();

        connect(_presetsComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &IPPropertyBinaryMorphologyTristateInt::setKernelPreset);

        _ignoreCombobox = false;
        _ignoreKernel = false;
    }

//    void

    void setMinimum(int)  {  }
    void setMaximum(int)  {  }
    std::vector<int> value()             { return _kernel; }

    void saveValue()        { _property->setValue(value()); }

signals:

public slots:
    void updateValue()
    {
        // any change should change the preset box to "Custom"
        if(!_ignoreCombobox)
            _presetsComboBox->setCurrentIndex(0);

        // clear old kernel
        _kernel.clear();

        int offset = 3;     // 3x3
        if(_kernelType == 1)
        {
            offset = 2;     // 5x5
        }
        else if(_kernelType == 2)
        {
            offset = 1;     // 7x7
        }
        else if(_kernelType == 3)
        {
            offset = 0;     // 9x9
        }

        // get all values from the active inputs
        for(int i=0; i<_inputs.size(); i++)
        {
            int column = i%9;
            int row    = i/9;

            if(column >= offset && row >= offset && column < 9-offset && row < 9-offset)
            {
                // unchecked = 0, checked = 1, dontcare = -1
                int value = 0;
                if(_inputs[i]->checkState() == Qt::PartiallyChecked)
                    value = -1;
                else if(_inputs[i]->checkState() == Qt::Checked)
                    value = 1;

                _kernel.push_back(value);
            }
        }

        // output
        QString tmp;
        for(size_t i=0; i<_kernel.size(); i++)
        {
            tmp += QString::number(_kernel[i]);
            if(i < _kernel.size()-1)
            {
                tmp += ",";
            }
        }

        if(_property)
            saveValue();

        emit changed();
    }

    void enableDisableKernelEditor()
    {
        _ignoreKernel = true;
        _kernelType = _kernelSizeComboBox->currentIndex();

        int offset = 3;     // 3x3
        if(_kernelType == 1)
        {
            offset = 2;     // 5x5
        }
        else if(_kernelType == 2)
        {
            offset = 1;     // 7x7
        }
        else if(_kernelType == 3)
        {
            offset = 0;     // 9x9
        }

        for(int i=0; i<_inputs.size(); i++)
        {
            int column = i%9;
            int row    = i/9;

            if(column >= offset && row >= offset && column < 9-offset && row < 9-offset)
            {
                _inputs[i]->setEnabled(true);
            }
            else
            {
                _inputs[i]->setEnabled(false);
            }
        }
        updateValue();
        _ignoreKernel = false;
    }

    void setKernelPreset()
    {
        int index = _presetsComboBox->currentIndex();

        // Custom should not overwrite the values
        if(index == 0)
        {
            return;
        }

        // compute offset
        int offset = 4-((int)sqrt((float)_presets[index].kernel.size()) / 2);

        // assign values to the input boxes
        int j = 0;
        _ignoreCombobox = true;
        for(int i=0; i<_inputs.size(); i++)
        {
            int column = i%9;
            int row    = i/9;

            if(column >= offset && row >= offset && column < 9-offset && row < 9-offset)
            {
                int state = _presets[index].kernel[j++];
                if(state == -1)
                    _inputs[i]->setCheckState(Qt::PartiallyChecked);
                else if(state == 1)
                    _inputs[i]->setCheckState(Qt::Checked);
                else
                    _inputs[i]->setCheckState(Qt::Unchecked);
            }
            else
            {
                _inputs[i]->setChecked(false);
            }
        }
        _ignoreCombobox = false;
        updateValue();
    }

private:
    IPLProcessPropertyVectorInt*   _property;
    QGridLayout*                _gridLayout;
    std::vector<int>            _kernel;
    int                         _kernelType;        ///< 0=3x3, 1=5x5, 2=7x7
    QList<QCheckBox*>           _inputs;
    QComboBox*                  _presetsComboBox;
    QComboBox*                  _kernelSizeComboBox;
    QWidget*                    _kernelWidget;
    bool                        _ignoreCombobox;
    bool                        _ignoreKernel;
    std::vector<IPKernelPreset> _presets;
};

#endif // IPPROPERTYBINARYMORPHOLOGYTRISTATEINT_H
