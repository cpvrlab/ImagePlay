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

#include "IPZoomWidget.h"

IPZoomWidget::IPZoomWidget(QWidget *parent) :
    QWidget(parent)
{
    _x = 0;
    _y = 0;
    _columnOffset = 1;
    _positionLocked = false;
    _image = NULL;

    _zoomUpdateMutex = new QMutex;
}

void IPZoomWidget::setImage(IPLData *data)
{
    _image = dynamic_cast<IPLImage*>(data);

    if(_image)
    {
        setVisible(true);
        update();
    }
    else
    {
        setVisible(false);
    }
}

void IPZoomWidget::setPosition(int x, int y)
{
    if(_positionLocked)
        return;

    _x = x;
    _y = y;

    // redraw
    update();
}

void IPZoomWidget::setColumnOffset(int offset)
{
    _columnOffset = offset;

    // redraw
    update();
}

int IPZoomWidget::columnOffset()
{
    return _columnOffset;
}

bool IPZoomWidget::isPositionLocked()
{
    return _positionLocked;
}

void IPZoomWidget::togglePositionLocked()
{
    _positionLocked = !_positionLocked;
}

void IPZoomWidget::setPositionLocked(bool locked)
{
    _positionLocked = locked;
}

void IPZoomWidget::paintEvent(QPaintEvent*)
{
    if(!_image)
        return;

    if(!_zoomUpdateMutex->tryLock())
        return;

    QPainter painter(this);
    QBrush brush(Qt::black);
    QColor highlightColor(Qt::red);

    int cellWidth = width()/(2*_columnOffset+1);

    int rectX = 0;
    int rectY = 0;
    for(int offsetY=-_columnOffset; offsetY <= _columnOffset; offsetY++)
    {
        for(int offsetX=-_columnOffset; offsetX <= _columnOffset; offsetX++)
        {
            uchar r = 0;
            uchar g = 0;
            uchar b = 0;

            int nrOfPlanes = _image->getNumberOfPlanes();
            if( _image->type() == IPL_IMAGE_COLOR)
            {
                //uchar test = *_image->rgb32() + (_x+offsetX)*_image->height() + (_y+offsetY);
                r = _image->plane(0)->cp(_x+offsetX, _y+offsetY) * FACTOR_TO_UCHAR;
                g = _image->plane(1)->cp(_x+offsetX, _y+offsetY) * FACTOR_TO_UCHAR;
                b = _image->plane(2)->cp(_x+offsetX, _y+offsetY) * FACTOR_TO_UCHAR;

                highlightColor = QColor(255-r, 255-g, 255-b);
            }
            else if( _image->type() == IPL_IMAGE_GRAYSCALE || _image->type() == IPL_IMAGE_BW )
            {
                r = g = b = _image->plane(0)->cp(_x+offsetX, _y+offsetY) * FACTOR_TO_UCHAR;

                highlightColor = QColor(255, 0, 0);
            }

            else if( _image->type() == IPL_IMAGE_ORIENTED )
            {
                r = g = b = _image->plane(0)->cp(_x+offsetX, _y+offsetY) * FACTOR_TO_UCHAR;

                highlightColor = QColor(255, 0, 0);
            }

            brush.setColor(QColor(r, g, b));
            painter.fillRect(rectX+1, rectY+1, cellWidth-1, cellWidth-1, brush);

            // center
            if(offsetX == 0 && offsetY == 0)
            {
                painter.setPen(highlightColor);
                painter.drawRect(rectX, rectY, cellWidth, cellWidth);
            }

            rectX += cellWidth;
        }
        rectX = 0;
        rectY += cellWidth;
    }
    _zoomUpdateMutex->unlock();
}
