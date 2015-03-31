//#############################################################################
//  File:      IPProcessStep.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPPROCESSSTEP_H
#define IPPROCESSSTEP_H

#include <QGraphicsItem>

#include <QDebug>
#include <QPainter>
#include <QTextItem>
#include <QGraphicsSceneDragDropEvent>
#include <QDebug>
#include <QPicture>
#include <QIcon>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QTimeLine>
#include <QGraphicsItemAnimation>

#include "IPProcessEdge.h"

#include "MainWindow.h"
#include "ConnectionDialog.h"

#include "IPL_processes.h"

class MainWindow;
class IPProcessEdge;

//-----------------------------------------------------------------------------
//!IPProcessStep represents an item in the process graph
/*!
 * References IPLProcess which contains the logic.
 */
class IPProcessStep : public QGraphicsItem
{
public:
                            IPProcessStep   (MainWindow* mainWindow, QString processName);
                            ~IPProcessStep  ();
    void                    paint           (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF                  boundingRect    () const;
    void                    snapToGrid      ();
    void                    setName         (QString name)          { _name = name; }
    QString                 name            ()                      { return _name; }
    void                    setIcon         (QIcon icon)            { _icon = icon; }
    bool                    isEditing       ()                      { return _editing; }
    void                    setEditing      (bool editing)          { _editing = editing; update(boundingRect()); }
    void                    setDuration     (int durationMs)        { _durationMs = durationMs; }
    IPLProcess*             process         ()                      { return _process; }
    QList<IPProcessEdge*>*  edgesIn         ()                      { return &_edgesIn; }
    QList<IPProcessEdge*>*  edgesOut        ()                      { return &_edgesOut; }
    int                     stepID          ()                      { return _stepID; }
    void                    setTreeDepth    (int treeDepth)         { _treeDepth = treeDepth; }
    int                     treeDepth       ()                      { return _treeDepth; }
    void                    setBranchID     (int branchId)          { _branchID = branchId; }
    int                     branchID        ()                      { return _branchID; }
    void                    setErrorMessage (QString error)         { _errorMessage = error; }
    QString                 errorMessage    ()                      { return _errorMessage; }
    void                    addEdgeIn       (IPProcessEdge* edge)   { _edgesIn.append(edge); }
    void                    removeEdgeIn    (IPProcessEdge* edge)   { _edgesIn.removeAll(edge); }
    void                    addEdgeOut      (IPProcessEdge* edge)   { _edgesOut.append(edge); }
    void                    removeEdgeOut   (IPProcessEdge* edge)   { _edgesOut.removeAll(edge); }
    void                    updateThumbnail ();

public slots:
    void                    setProgress     (int progress);

private:
    static long             _stepCounter;   //!< auto increment for step ID
    long                    _stepID;
    QString                 _name;
    QString                 _errorMessage;
    QIcon                   _icon;
    QIcon                   _openCvIcon;
    QPixmap                 _thumbnail;
    MainWindow*             _mainWindow;
    IPLProcess*             _process;
    QPointF                 _lastPosition;
    bool                    _editing;
    bool                    _connecting;
    int                     _durationMs;
    int                     _treeDepth;
    int                     _branchID;
    int                     _progress;      //!< from 0 to 100%
    QList<IPProcessEdge*>   _edgesIn;
    QList<IPProcessEdge*>   _edgesOut;

    // QGraphicsItem interface
protected:
    void                    mousePressEvent         (QGraphicsSceneMouseEvent *event);
    void                    mouseReleaseEvent       (QGraphicsSceneMouseEvent *event);
    void                    mouseMoveEvent          (QGraphicsSceneMouseEvent *event);
    void                    mouseDoubleClickEvent   (QGraphicsSceneMouseEvent *event);

    QVariant                itemChange              (GraphicsItemChange change, const QVariant &value);
};

#endif // IPPROCESSSTEP_H
