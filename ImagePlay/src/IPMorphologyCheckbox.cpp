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

#include "IPMorphologyCheckbox.h"

IPMorphologyCheckbox::IPMorphologyCheckbox(QWidget *parent) :
    QCheckBox(parent)
{
//    setMouseTracking(true);
}


void IPMorphologyCheckbox::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor fillColor = Qt::black;    // 0
    if(checkState() == Qt::PartiallyChecked)
    {
        fillColor = Qt::lightGray;      // 1
    }
    else if(checkState() == Qt::Checked)
    {
        fillColor = Qt::white;          // 2
    }

    // invisible if disabled
    if(isEnabled())
    {
        QBrush brush(fillColor);

        painter.fillRect(0, 0, width(), height(), brush);
        painter.drawRect(0, 0, width(), height());
    }
    else
    {
        QPen pen(Qt::lightGray);
        painter.setPen(pen);
        painter.drawRect(0, 0, width(), height());
    }
}


// enhance weird click behaviour
void IPMorphologyCheckbox::mousePressEvent(QMouseEvent *)
{
    qDebug() << "mousePressEvent";
    click();
}

void IPMorphologyCheckbox::enterEvent(QEvent*)
{
//    qDebug() << "enterEvent";
//    click();
}

