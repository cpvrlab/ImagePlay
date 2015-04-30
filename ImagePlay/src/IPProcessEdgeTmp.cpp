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

#include "IPProcessEdgeTmp.h"

const qreal Pi = 3.14;

IPProcessEdgeTmp::IPProcessEdgeTmp()
{
    setVisible(false);
    setZValue(-50);
}

QRectF IPProcessEdgeTmp::boundingRect() const
{
    return QRectF(_from, _to);
}

void IPProcessEdgeTmp::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    // main line
    QPen pen(Qt::DotLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(2);
    pen.setColor(Qt::red);

    painter->setPen(pen);
    QLineF line(_from.toPoint()+QPoint(32,32), _to.toPoint());

    painter->drawLine(line);

    // arrow
    QPolygonF arrowPolygon;
    double arrowSize = 10;
    double angle = acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = (Pi * 2) - angle;

    QPointF arrowP1 = _to - QPointF(sin(angle + Pi / 3) * arrowSize,
                                           cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = _to - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                           cos(angle + Pi - Pi / 3) * arrowSize);
    arrowPolygon.clear();
    arrowPolygon << _to << arrowP1 << arrowP2;

    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->drawPolygon(arrowPolygon);
}
