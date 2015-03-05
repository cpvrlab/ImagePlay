//#############################################################################
//  File:      IPProcessGridScene.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPPROCESSGRIDSCENE_H
#define IPPROCESSGRIDSCENE_H

#include <QGraphicsScene>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
#include <QListWidgetItem>

#include <QDebug>

#include "IPProcessEdgeTmp.h"
#include "IPProcessStep.h"
#include "IPProcessEdge.h"

#include "MainWindow.h"
#include "IPImageViewer.h"
#include "IPProcessGrid.h"

class IPProcessEdge;

//-----------------------------------------------------------------------------
//!IPProcessGridScene holds all processsteps and connections
/*!
 *
 */
class IPProcessGridScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit                IPProcessGridScene      (QObject *parent = 0);
                            ~IPProcessGridScene     ();
    void                    snapSelectedItemsToGrid ();
    void                    deleteSelectedItems     ();
    void                    addStep                 (IPProcessStep* step);
    void                    removeStep              (IPProcessStep* step);
    bool                    addEdge                 (IPProcessEdge* edge);
    void                    removeEdge              (IPProcessEdge* edge);
    void                    setTmpArrowFrom         (QPointF pos)           { _tmpArrow->setFrom(pos); }
    void                    setTmpArrowTo           (QPointF pos)           { _tmpArrow->setTo(pos); }
    void                    setTmpArrowVisible      (bool visible)          { _tmpArrow->setVisible(visible); }
    void                    setShowThumbnails       (bool showThumbnails)   { _showThumbnails = showThumbnails; }
    bool                    showThumbnails          ()                      { return _showThumbnails; }
    QList<IPProcessStep*>*  steps                   ()                      { return &_steps; }
    QList<IPProcessEdge*>*  edges                   ()                      { return &_edges; }

    // QGraphicsScene interface
protected:
    void                    dragEnterEvent          (QGraphicsSceneDragDropEvent *event);
    void                    dragMoveEvent           (QGraphicsSceneDragDropEvent *event);
    void                    dropEvent               (QGraphicsSceneDragDropEvent *event);
private:
    QList<IPProcessStep*>   _steps;                 //!< List of steps
    QList<IPProcessEdge*>   _edges;                 //!< List of connections
    IPProcessEdgeTmp*       _tmpArrow;              //!< Temporary arrow while dragging the mouse
    bool                    _showThumbnails;        //!< Normal or thumbnail rendering mode
};

#endif // IPPROCESSGRIDSCENE_H
