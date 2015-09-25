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

#ifndef IPZOOMWIDGET_H
#define IPZOOMWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QRect>
#include <QDebug>

#include "IPL_processes.h"
#include "IPImageViewer.h"

//-----------------------------------------------------------------------------
//!IPZoomWidget renders a small zoom window of a IPImageViewer
/*!
 *
 */
class IPZoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit    IPZoomWidget            (QWidget *parent = 0);
    void        setImage                (IPLData* data);
    void        setPosition             (int x, int y);
    void        setColumnOffset         (int offset);
    int         columnOffset            ();
    bool        isPositionLocked        ();
    void        togglePositionLocked    ();
    void        setPositionLocked       (bool locked);

private:
    IPLImage*   _image;
    int         _x;
    int         _y;
    int         _columnOffset;
    bool        _positionLocked;


    // QWidget interface
protected:
    void        paintEvent              (QPaintEvent *);
};

#endif // IPZOOMWIDGET_H
