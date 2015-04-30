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

#ifndef IPPROCESSEDGETMP_H
#define IPPROCESSEDGETMP_H

#include <QGraphicsItem>
#include <QPainter>

#include <QDebug>

#include <cmath>

//-----------------------------------------------------------------------------
//!Simple process edge while draging with the mouse
/*!
 * Uses coordinates instead of IPProcessSteps as start end end points.
 */
class IPProcessEdgeTmp : public QGraphicsItem
{
public:
    explicit            IPProcessEdgeTmp();
    void                paint           (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF              boundingRect    () const;

    void                setFrom         (QPointF from)      { _from = from; }
    void                setTo           (QPointF to)        { _to = to; update(0,0,1000,1000); }

private:
    QPointF             _from;          //!< First coordinates
    QPointF             _to;            //!< Second coordinates
};

#endif // IPPROCESSEDGETMP_H
