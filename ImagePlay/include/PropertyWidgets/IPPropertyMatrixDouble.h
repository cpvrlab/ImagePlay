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

#ifndef IPPropertyMatrixDouble_H
#define IPPropertyMatrixDouble_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QComboBox>

#include <QDebug>

#include "IPPropertyWidget.h"

#include "IPKernelPreset.h"

class IPPropertyMatrixDouble : public IPPropertyWidget
{
    Q_OBJECT
public:
    IPPropertyMatrixDouble(IPLProcessPropertyVectorDouble* property, QWidget *parent) : IPPropertyWidget(property, parent)
    {
        // get property values
        _property   = property;
        _matrix     = ((IPLProcessPropertyVectorDouble*) property)->value();

        // split the properties
        QString rawName(property->title());

        // check if structure is Title:Option1|Option2
        if(!(rawName.contains(":") && rawName.contains("|")))
        {
            qWarning() << "IPPropertyMatrixDouble: Invalid title structure " << rawName;
            return;
        }

        QString name = rawName.split(":").at(0);
        QString rawOptions = rawName.split(":").at(1);
        QStringList options = rawOptions.split("|");

        QString sRows(options[0]);
        QString sCols(options[1]);

        _rows = sRows.toInt();
        _cols = sCols.toInt();

        _ignoreUpdates = true;

        setLayout(new QVBoxLayout(this));
        _gridLayout = new QGridLayout;
        _gridLayout->setSpacing(2);

        _matrixWidget = new QWidget(this);
        _matrixWidget->setLayout(_gridLayout);
        layout()->addWidget(_matrixWidget );

        for(int i=0; i<_matrix.size(); i++)
        {
            int column = i%_cols;
            int row    = i/_cols;

            QDoubleSpinBox* input = new QDoubleSpinBox(this);
            input->setButtonSymbols(QAbstractSpinBox::NoButtons);
            input->setAlignment(Qt::AlignCenter);
            input->setMinimumWidth(16);
            input->setMinimumHeight(20);

            input->setMinimum(-1000);
            input->setMaximum(1000);
            input->setSingleStep(0.5);
            input->setDecimals(3);

            _gridLayout->addWidget(input, row, column);
            _inputs.push_back(input);

            connect(input, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &IPPropertyMatrixDouble::updateValue);
        }
        init();


        _ignoreUpdates = false;
    }

    void init()
    {
        _ignoreUpdates = true;
        _matrix = _property->value();

        int i = 0;
        for(QDoubleSpinBox* input : _inputs)
        {
            input->setValue(_matrix[i++]);
        }
        _ignoreUpdates = false;
    }

    void setMinimum(int)  {  }
    void setMaximum(int)  {  }
    std::vector<double> value()             { return _matrix; }

    void saveValue()        { _property->setValue(value()); }
    void resetValue()       { _property->resetValue(); init(); }

signals:

public slots:
    void updateValue()
    {
        if(_ignoreUpdates)
            return;

        // clear old kernel
        _matrix.clear();

        // get all values from the active inputs
        for(int i=0; i<_inputs.size(); i++)
        {
            _matrix.push_back(_inputs[i]->value());
        }

        if(_property)
            saveValue();

        emit changed();
    }

private:
    IPLProcessPropertyVectorDouble*   _property;
    QGridLayout*                _gridLayout;
    std::vector<double>         _matrix;
    int                         _cols;
    int                         _rows;
    QList<QDoubleSpinBox*>      _inputs;
    QWidget*                    _matrixWidget;
    bool                        _ignoreUpdates;
};

#endif // IPPropertyMatrixDouble_H
