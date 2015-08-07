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

#ifndef IPPROCESSGRIDSCENE_H
#define IPPROCESSGRIDSCENE_H

#include <QGraphicsScene>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
#include <QListWidgetItem>
#include <QMimeDatabase>
#include <QMimeType>

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
    bool                    isSupportedMimeType     (const QMimeData* mimeData);
    IPProcessStep*          createProcessStep       (QString processID, QPointF scenePos);

private:
    QList<IPProcessStep*>   _steps;                 //!< List of steps
    QList<IPProcessEdge*>   _edges;                 //!< List of connections
    IPProcessEdgeTmp*       _tmpArrow;              //!< Temporary arrow while dragging the mouse
    bool                    _showThumbnails;        //!< Normal or thumbnail rendering mode
};

#endif // IPPROCESSGRIDSCENE_H
