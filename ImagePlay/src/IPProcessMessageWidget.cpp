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

#include "IPProcessMessageWidget.h"

IPProcessMessageWidget::IPProcessMessageWidget(QWidget *parent) :
    QWidget(parent)
{
    _mainWindow = NULL;
    _processStep = NULL;

    // add settings to panel
    setLayout(new QVBoxLayout);
    this->layout()->setMargin(0);
    //setBackgroundRole(QPalette::Midlight);
}

void IPProcessMessageWidget::init(IPProcessStep* processStep)
{
    _processStep = processStep;

    updateMessages();
}

void IPProcessMessageWidget::updateMessages()
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

        // add error messages
        if(_processStep->process()->hasMessages())
        {
            for(IPLProcessMessage processMessage: *_processStep->process()->messages())
            {
                if(processMessage.type == IPLProcessMessage::ERR)
                {
                    QString msgString("<b>Error: </b> %1");
                    QLabel* msgLabel = new QLabel(msgString.arg(QString::fromStdString(processMessage.msg)).replace("\n", "<br>"), this);
                    msgLabel->setWordWrap(true);
                    msgLabel->setStyleSheet("color: #a94442; background-color: #f2dede; border: 1px solid #ebccd1; padding:4px; margin: 2px 0px 2px 0px;");
                    this->layout()->addWidget(msgLabel);
                }
                else if(processMessage.type == IPLProcessMessage::WARNING)
                {
                    QString msgString("<b>Warning: </b> %1");
                    QLabel* msgLabel = new QLabel(msgString.arg(QString::fromStdString(processMessage.msg)).replace("\n", "<br>"), this);
                    msgLabel->setWordWrap(true);
                    msgLabel->setStyleSheet("color: #8a6d3b; background-color: #fcf8e3; border: 1px solid #faebcc; padding:4px; margin: 2px 0px 2px 0px;");
                    this->layout()->addWidget(msgLabel);
                }
                else if(processMessage.type == IPLProcessMessage::SUCCESS)
                {
                    QString msgString("%1");
                    QLabel* msgLabel = new QLabel(msgString.arg(QString::fromStdString(processMessage.msg)).replace("\n", "<br>"), this);
                    msgLabel->setWordWrap(true);
                    msgLabel->setStyleSheet("color: #3c763d; background-color: #dff0d8; border: 1px solid #d6e9c6; padding:4px; margin: 2px 0px 2px 0px;");
                    this->layout()->addWidget(msgLabel);
                }
                else if(processMessage.type == IPLProcessMessage::INFORMATION)
                {
                    QString msgString("%1");
                    QLabel* msgLabel = new QLabel(msgString.arg(QString::fromStdString(processMessage.msg)).replace("\n", "<br>"), this);
                    msgLabel->setWordWrap(true);
                    msgLabel->setStyleSheet("color: #31708f; background-color: #d9edf7; border: 1px solid #bce8f1; padding:4px; margin: 2px 0px 2px 0px;");
                    this->layout()->addWidget(msgLabel);
                }
            }
        }
    }
}
