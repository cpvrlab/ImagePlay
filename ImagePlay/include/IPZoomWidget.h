//#############################################################################
//  File:      IPZoomWidget.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPZOOMWIDGET_H
#define IPZOOMWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QRect>
#include <QDebug>

#include "IPL_processes.h"

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
    void        setImage                (IPLImage* image);
    void        setPosition             (int x, int y);
    void        setColumnOffset         (int offset);
    int         columnOffset            ();
    bool        isPositionLocked        ();
    void        togglePositionLocked    ();
    void        setPositionLocked       (bool locked);

private:
    QScopedPointer<IPLImage> _image;
    int                      _x;
    int                      _y;
    int                      _columnOffset;
    bool                     _positionLocked;


    // QWidget interface
protected:
    void        paintEvent              (QPaintEvent *);
};

#endif // IPZOOMWIDGET_H
