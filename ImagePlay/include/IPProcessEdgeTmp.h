//#############################################################################
//  File:      IPProcessEdgeTmp.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
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
