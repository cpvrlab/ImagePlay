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

#include "IPProcessInputOutputWidget.h"

IPProcessInputOutputWidget::IPProcessInputOutputWidget(QWidget *parent) :
    QWidget(parent)
{
    _mainWindow = NULL;
    _processStep = NULL;

    // add settings to panel
    setLayout(new QVBoxLayout);
    this->layout()->setMargin(0);
    //setBackgroundRole(QPalette::Midlight);
}

void IPProcessInputOutputWidget::init(IPProcessStep* processStep)
{
    _processStep = processStep;

    updateInputsOutputs();
}

void IPProcessInputOutputWidget::updateInputsOutputs()
{
    if(_processStep && isVisible())
    {
        // remove all children
        while (layout()->count() > 0)
        {
            QLayoutItem* item = layout()->takeAt(0);
            if(item != NULL)
            {
                delete item->widget();
                delete item;
            }
        }
//      delete layout();

        // add inputs
        for(IPLProcessIO input: *_processStep->process()->inputs())
        {
            //QString tmp = QString::fromStdString(dataTypeName(input.type));
            QString msgString("<b>Input %1: </b> %2");
            QLabel* msgLabel = new QLabel(msgString.arg(input.index).arg(input.type), this);
            msgLabel->setWordWrap(true);
            msgLabel->setStyleSheet("color: #a94442; background-color: #f2dede; border: 1px solid #ebccd1; padding:4px; margin: 2px 0px 2px 0px;");
            this->layout()->addWidget(msgLabel);
        }

        // add outputs
        for(IPLProcessIO output: *_processStep->process()->outputs())
        {
            //QString tmp = QString::fromStdString(dataTypeName(output.type));
            QString msgString("<b>Output %1: </b> %2");
            QLabel* msgLabel = new QLabel(msgString.arg(output.index).arg(output.type), this);
            msgLabel->setWordWrap(true);
            msgLabel->setStyleSheet("color: #a94442; background-color: #f2dede; border: 1px solid #ebccd1; padding:4px; margin: 2px 0px 2px 0px;");
            this->layout()->addWidget(msgLabel);
        }
    }
}
