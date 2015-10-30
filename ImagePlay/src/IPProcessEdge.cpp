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

#include "IPProcessEdge.h"

const qreal Pi = 3.14;

IPProcessEdge::IPProcessEdge(IPProcessStep* from, IPProcessStep* to, int indexFrom, int indexTo)
{
    _from = from;
    _to = to;
    _indexFrom = indexFrom;
    _indexTo = indexTo;
    setZValue(-100);

    // colors
    // BW, GRAY, COLOR, UNDEFINED
    _colorArrow.push_back(QColor(0, 0, 0));         // BW: black
    _colorArrow.push_back(QColor(255, 255, 255));   // GRAY: white
    _colorArrow.push_back(QColor(46, 204, 113));    // COLOR: purple
    _colorArrow.push_back(QColor(255, 0, 0));       // COLOR: red
    _colorArrow.push_back(QColor(241, 196, 15));    // UNDEFINED: yellow

    // set QGraphicItem properties
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);

    _mouseOver = false;

//    _typeIndex   = _from->process()->outputs()->at(_indexFrom).type;

    updatePosition();
}

IPProcessEdge::~IPProcessEdge()
{
    _from->removeEdgeOut(this);
    _to->removeEdgeIn(this);
}

QRectF IPProcessEdge::boundingRect() const
{
    float minX = std::min(_intersectionFrom.x(), _intersectionTo.x());
    float maxX = std::max(_intersectionFrom.x(), _intersectionTo.x());
    float minY = std::min(_intersectionFrom.y(), _intersectionTo.y());
    float maxY = std::max(_intersectionFrom.y(), _intersectionTo.y());

    // add some margin for straight vertical or horizontal lines
    minX -= 10;
    maxX += 10;
    minY -= 10;
    maxY += 10;

    float width = std::abs(maxX-minX);
    float height = std::abs(maxY-minY);

    return QRectF(minX, minY, width, height);
}

QPainterPath IPProcessEdge::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void IPProcessEdge::updatePosition()
{
    _typeIndex   = _from->process()->outputs()->at(_indexFrom).type;

    // main line
    QLineF line(_from->pos().toPoint()+QPoint(32,32), _to->pos().toPoint()+QPoint(32,32));

    // find intersections for arrows
    QRectF boundingRect = _to->boundingRect();
    QList<QLineF> lines;
    lines.append(QLineF(_to->pos()+boundingRect.topLeft(),      _to->pos()+boundingRect.topRight()));
    lines.append(QLineF(_to->pos()+boundingRect.topRight(),     _to->pos()+boundingRect.bottomRight()));
    lines.append(QLineF(_to->pos()+boundingRect.bottomRight(),  _to->pos()+boundingRect.bottomLeft()));
    lines.append(QLineF(_to->pos()+boundingRect.bottomLeft(),   _to->pos()+boundingRect.topLeft()));

    for (int i=0; i<lines.length(); i++) {
        QLineF::IntersectType intersectType = line.intersect(lines[i], &_intersectionTo);
        if (intersectType == QLineF::BoundedIntersection)
            break;
    }

    // 2
    QRectF boundingRect2 = _from->boundingRect();
    QList<QLineF> lines2;
    lines2.append(QLineF(_from->pos()+boundingRect2.topLeft(),      _from->pos()+boundingRect2.topRight()));
    lines2.append(QLineF(_from->pos()+boundingRect2.topRight(),     _from->pos()+boundingRect2.bottomRight()));
    lines2.append(QLineF(_from->pos()+boundingRect2.bottomRight(),  _from->pos()+boundingRect2.bottomLeft()));
    lines2.append(QLineF(_from->pos()+boundingRect2.bottomLeft(),   _from->pos()+boundingRect2.topLeft()));

    for (int i=0; i<lines2.length(); i++) {
        QLineF::IntersectType intersectType = line.intersect(lines2[i], &_intersectionFrom);
        if (intersectType == QLineF::BoundedIntersection)
            break;
    }
}

void IPProcessEdge::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget *)
{
    painter->setBrush(QBrush(_colorArrow.at(_typeIndex)));
    painter->setPen(QColor(_colorArrow.at(_typeIndex)));

    // main line
    if(_mouseOver) painter->setPen(QColor(192, 57, 43));

    QLineF line2(_intersectionFrom.toPoint(), _intersectionTo.toPoint());
    painter->drawLine(line2);

    // arrow
    QPolygonF arrowPolygon;
    double arrowSize = 10;
    double angle = ::acos(line2.dx() / line2.length());
    if (line2.dy() >= 0)
        angle = (Pi * 2) - angle;

    QPointF arrowP1 = _intersectionTo - QPointF(sin(angle + Pi / 3) * arrowSize,
                                           cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = _intersectionTo - QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                           cos(angle + Pi - Pi / 3) * arrowSize);
    arrowPolygon.clear();
    arrowPolygon << _intersectionTo << arrowP1 << arrowP2;

    // point at process item 1
    painter->drawEllipse(_intersectionFrom, 4, 4);

    // arrow at process item 2
    painter->drawPolygon(arrowPolygon);

    if(_mouseOver)
    {
        // draw cross
        QPointF centerPoint = _intersectionFrom + 0.5*(_intersectionTo-_intersectionFrom);

        QLineF line1(centerPoint+QPointF(-5,-5), centerPoint+QPointF(5,5));
        QLineF line2(centerPoint+QPointF(-5,5), centerPoint+QPointF(5,-5));

        QPen penRed(Qt::red);
        penRed.setWidth(2);

        painter->setPen(penRed);
        painter->drawLine(line1);
        painter->drawLine(line2);
    }
}


void IPProcessEdge::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void IPProcessEdge::mouseReleaseEvent(QGraphicsSceneMouseEvent*)
{
    if(_mouseOver)
    {
        ((IPProcessGridScene*) scene())->removeEdge(this);
    }
}

void IPProcessEdge::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
}

void IPProcessEdge::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    _mouseOver = false;
    QGraphicsItem::hoverLeaveEvent(event);
}

void IPProcessEdge::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    // calculate distance from line to point
    int x0 = event->scenePos().x();
    int y0 = event->scenePos().y();
    int x1 = _intersectionFrom.toPoint().x();
    int y1 = _intersectionFrom.toPoint().y();
    int x2 = _intersectionTo.toPoint().x();
    int y2 = _intersectionTo.toPoint().y();

    int v1 = (x2-x1)*(y1-y0) - (x1-x0)*(y2-y1);
    int v2 = ::sqrt((double) ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
    int distance = ::abs(v1)/v2;

    if(distance < 20)
    {
        _mouseOver = true;
    }
    else
    {
        _mouseOver = false;
    }

    // redraw
    update(boundingRect());

    QGraphicsItem::hoverMoveEvent(event);
}


QVariant IPProcessEdge::itemChange(GraphicsItemChange change, const QVariant &value)
{
    // prevent selection state
    if(change == ItemSelectedChange)
    {
        return false;
    }
    return QGraphicsItem::itemChange(change, value);
}
