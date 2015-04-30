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

#include "IPFilterLineEdit.h"

IPFilterLineEdit::IPFilterLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    _resetButton = new QPushButton("", this);
    _resetButton->setMinimumSize(QSize(16,16));
    _resetButton->setMaximumSize(QSize(16,16));
    _resetButton->setIcon(QIcon(":/Delete-32.png"));
    _resetButton->setStyleSheet("border:none;");
    setCursor(Qt::ArrowCursor);

    setLayout(new QHBoxLayout);

    this->layout()->addWidget(_resetButton);
    this->layout()->setContentsMargins(0,0,3,0);
    this->layout()->setAlignment(_resetButton, Qt::AlignRight);

    connect(_resetButton, &QPushButton::pressed, this, &IPFilterLineEdit::resetPressed);
    connect(_resetButton, &QPushButton::released, this, &IPFilterLineEdit::resetReleased);
    connect(_resetButton, &QPushButton::clicked, this, &IPFilterLineEdit::resetClicked);
}

void IPFilterLineEdit::resetPressed()
{
    _resetButton->setIcon(QIcon(":/Delete-32-over.png"));
}

void IPFilterLineEdit::resetReleased()
{
    _resetButton->setIcon(QIcon(":/Delete-32.png"));
}

void IPFilterLineEdit::resetClicked()
{
    setText("");
}
