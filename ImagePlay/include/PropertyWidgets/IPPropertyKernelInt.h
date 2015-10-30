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

#ifndef IPPROPERTYKERNELINT_H
#define IPPROPERTYKERNELINT_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QSpinBox>
#include <QComboBox>

#include <QDebug>

#include "IPPropertyWidget.h"

#include "IPKernelPreset.h"

class IPPropertyKernelInt : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyKernelInt(IPLProcessPropertyVectorInt* property, QWidget *parent) : IPPropertyWidget(property, parent)
    {
        // init presets
        int presetEmpty[]        = {0,0,0,0,0,0,0,0,0};
        int presetDefault[]      = {0,0,0,0,1,0,0,0,0};
        int presetSharpen[]      = {0,-1,0,-1,5,-1,0,-1,0};
        int presetBoxBlur[]      = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
        int presetGaussianBlur3[]= {1,2,1,2,4,2,1,2,1};
        int presetGaussianBlur5[]= {1,4,6,4,1,4,16,24,16,4,6,24,36,24,6,4,16,24,16,4,1,4,6,4,1};
        int presetLoG3[]         = {-1,2,-1,2,-4,2,-1,2,-1};
        int presetLoG5[]         = {0,0,1,0,0,0,1,2,1,0,1,2,-16,2,1,0,1,2,1,0,0,0,1,0,0};
        int presetEdgeEnhance[]  = {0,0,0,-1,1,0,0,0,0};
        int presetEdgeDetect[]   = {0,1,0,1,4,1,0,1,0};
        int presetEmboss[]       = {-2,-1,0,-1,1,1,0,1,2};

        // add to preset list
        _presets.push_back(IPKernelPreset("Custom",         presetEmpty, 9));
        _presets.push_back(IPKernelPreset("Default",        presetDefault, 9));
        _presets.push_back(IPKernelPreset("Sharpen",        presetSharpen, 9));
        _presets.push_back(IPKernelPreset("Box Blur",       presetBoxBlur, 49));
        _presets.push_back(IPKernelPreset("Gaussian Blur 3x3",  presetGaussianBlur3, 9));
        _presets.push_back(IPKernelPreset("Gaussian Blur 5x5",  presetGaussianBlur5, 25));
        _presets.push_back(IPKernelPreset("Laplace of Gaussian 3x3",  presetLoG3, 9));
        _presets.push_back(IPKernelPreset("Laplace of Gaussian 5x5",  presetLoG5, 25));
        _presets.push_back(IPKernelPreset("Edge Enhance",   presetEdgeEnhance, 9));
        _presets.push_back(IPKernelPreset("Edge Detect",    presetEdgeDetect, 9));
        _presets.push_back(IPKernelPreset("Emboss",         presetEmboss, 9));

        // get property values
        _property = property;
        _kernel = ((IPLProcessPropertyVectorInt*) property)->value();

        _ignoreCombobox = false;

        _ignoreUpdates = true;

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
        connect(_kernelSizeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &IPPropertyKernelInt::enableDisableKernelEditor);

        layout()->addWidget(_kernelSizeComboBox);


        // compute offset
        int offset = 3-((int)sqrt((float)_kernel.size()) / 2);
        int j = 0;
        for(int i=0; i<49; i++)
        {
            int column = i%7;
            int row    = i/7;

            QSpinBox* input = new QSpinBox(this);
            input->setButtonSymbols(QAbstractSpinBox::NoButtons);
            input->setAlignment(Qt::AlignCenter);
            input->setMinimumWidth(16);
            input->setMinimumHeight(20);

            input->setMinimum(-128);
            input->setMaximum(128);

            _gridLayout->addWidget(input, i/7, i%7);
            _inputs.push_back(input);

            connect(input, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IPPropertyKernelInt::updateValue);
        }

        _presetsComboBox = new QComboBox(this);
        for(size_t i=0; i<_presets.size(); i++)
        {
            _presetsComboBox->addItem(QString::fromStdString(_presets[i].name));
        }
        connect(_presetsComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &IPPropertyKernelInt::setKernelPreset);
        layout()->addWidget(_presetsComboBox);

        init();

        enableDisableKernelEditor();
        setKernelPreset();

        _ignoreUpdates = false;
    }

    void init()
    {
         _kernel = _property->value();


         _kernelType = 0;        // 3x3
         if(_kernel.size() == 25)
         {
             _kernelType = 1;    // 5x5
         }
         else if(_kernel.size() == 49)
         {
             _kernelType = 2;    // 7x7
         }
         _kernelSizeComboBox->setCurrentIndex(_kernelType);

         int j = 0;
         int offset = 3-((int)sqrt((float)_kernel.size()) / 2);
         for(int i=0; i < _inputs.size(); i++)
         {
             QSpinBox* input = _inputs.at(i);
             int column = i%7;
             int row    = i/7;

             if(column >= offset && row >= offset && column < 7-offset && row < 7-offset)
             {
                 input->setValue(_kernel[j++]);
             }
             else
             {
                 input->setValue(0);
             }
         }
    }

    void setMinimum(int)  {  }
    void setMaximum(int)  {  }
    std::vector<int> value()             { return _kernel; }

    void saveValue()        { _property->setValue(value()); }
    void resetValue()       { _property->resetValue(); init(); }

signals:

public slots:
    void updateValue()
    {
        if(_ignoreUpdates)
            return;

        // any change should change the preset box to "Custom"
        if(!_ignoreCombobox)
            _presetsComboBox->setCurrentIndex(0);

        // clear old kernel
        _kernel.clear();

        int offset = 2;     // 3x3
        if(_kernelType == 1)
        {
            offset = 1;     // 5x5
        }
        else if(_kernelType == 2)
        {
            offset = 0;     // 7x7
        }

        // get all values from the active inputs
        for(int i=0; i<_inputs.size(); i++)
        {
            int column = i%7;
            int row    = i/7;

            if(column >= offset && row >= offset && column < 7-offset && row < 7-offset)
            {
                _kernel.push_back(_inputs[i]->value());
            }
        }

        if(_property)
            saveValue();

        emit changed();
    }

    void enableDisableKernelEditor()
    {
        _kernelType = _kernelSizeComboBox->currentIndex();

        int offset = 2;     // 3x3
        if(_kernelType == 1)
        {
            offset = 1;     // 5x5
        }
        else if(_kernelType == 2)
        {
            offset = 0;     // 7x7
        }

        for(int i=0; i<_inputs.size(); i++)
        {
            int column = i%7;
            int row    = i/7;

            if(column >= offset && row >= offset && column < 7-offset && row < 7-offset)
            {
                _inputs[i]->setEnabled(true);
            }
            else
            {
                _inputs[i]->setEnabled(false);
            }
        }
        updateValue();
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
        int offset = 3-((int)sqrt((float)_presets[index].kernel.size()) / 2);

        // assign values to the input boxes
        int j = 0;
        _ignoreCombobox = true;
        for(int i=0; i<_inputs.size(); i++)
        {
            int column = i%7;
            int row    = i/7;

            if(column >= offset && row >= offset && column < 7-offset && row < 7-offset)
            {
                _inputs[i]->setValue(_presets[index].kernel[j++]);
            }
            else
            {
                _inputs[i]->setValue(0);
            }
        }
        updateValue();
        _ignoreCombobox = false;
    }

private:
    IPLProcessPropertyVectorInt*   _property;
    QGridLayout*                _gridLayout;
    std::vector<int>            _kernel;
    int                         _kernelType;        ///< 0=3x3, 1=5x5, 2=7x7
    QList<QSpinBox*>            _inputs;
    QComboBox*                  _presetsComboBox;
    QComboBox*                  _kernelSizeComboBox;
    QWidget*                    _kernelWidget;
    bool                        _ignoreCombobox;
    bool                        _ignoreUpdates;
    std::vector<IPKernelPreset> _presets;
};

#endif // IPPROPERTYKERNELINT_H
