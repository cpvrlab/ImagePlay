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

#ifndef IPPROCESSGRID_H
#define IPPROCESSGRID_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDragEnterEvent>
#include <QMainWindow>
#include <QStatusBar>
#include <QScrollBar>
#include <QQueue>
#include <QElapsedTimer>
#include <QApplication>

#include "IPProcessStep.h"
#include "IPProcessGridScene.h"
#include "IPProcessThread.h"

class IPProcessGridScene;
class MainWindow;

//-----------------------------------------------------------------------------
//!IPProcessGrid represents the process graph
/*!
 * This custom QGraphicsView manages the proceses, connections and renders
 * everything as a process graph.
 * All items are stored in IPProcessGridScene.
 * The graph traversation and process exectution is also handled here.
 */
class IPProcessGrid : public QGraphicsView, public IPLPropertyChangedEventHandler, public IPLOutputsChangedEventHandler
{
    Q_OBJECT
public:
    explicit                IPProcessGrid           (QWidget *parent = 0);
    void                    zoomIn                  ();
    void                    zoomOut                 ();
    void                    zoomBy                  (float scaleChange);
    void                    zoomTo                  (float scale);
    static bool             sortTreeDepthLessThan   (IPProcessStep* s1, IPProcessStep* s2);
    void                    buildQueue              ();
    int                     executeThread           (IPLProcess* process, IPLImage *image = NULL, int inputIndex = 0, bool useOpenCV = false);
    void                    propagateNeedsUpdate    (IPLProcess* process);
    void                    propagateResultReady    (IPLProcess *process, bool resultReady);
    void                    propertyChanged         (IPLProcess *);
    void                    outputsChanged          (IPLProcess *);
    void                    setSequenceIndex        (int index);
    void                    setSequenceRunning      (bool status)                           { _isSequenceRunning = status; }
    void                    setMainWindow           (MainWindow* mainWindow)                { _mainWindow = mainWindow; }
    void                    requestUpdate           ();
    MainWindow*             mainWindow              ()                                      { return _mainWindow; }
    IPProcessGridScene*     scene                   ()                                      { return _scene; }
    void                    stopExecution           ()                                      { _stopExecution = true; }

signals:
    void                    sequenceChanged         (int index, int count);

public slots:
    void                    execute                 (bool forcedUpdate = false);
    void                    updateProgress          (int);
    void                    sceneRectChanged        (const QRectF & rect);

private:
    void                    fitLargeSceneRect();

    IPProcessGridScene*     _scene;                 //!< Scene
    float                   _scale;                 //!< Scale for zooming
    MainWindow*             _mainWindow;            //!< MainWindow
    bool                    _isRunning;             //!< Is running
    bool                    _updateNeeded;
    IPProcessStep*          _currentStep;           //!< Currently active step, settings shown on the left side
    QList<IPProcessStep*>   _processList;           //!< Ordered process list
    int                     _sequenceCount;         //!< Image sequence count
    int                     _sequenceIndex;         //!< Current image sequence index
    int                     _lastSequenceIndex;     //!< Last image sequence index
    bool                    _isSequenceRunning;     //!< Is sequence running
    bool                    _lastProcessSuccess;    //!< Last process was successful
    bool                    _stopExecution;         //!< Used to stop the execution early
    bool                    _longProcess;           //!< Unmeasurable processes must update GUI regularly

    // QWidget interface
protected:
    void                    wheelEvent              (QWheelEvent *);
    virtual void            resizeEvent             (QResizeEvent *)                        override;
    void                    keyPressEvent           (QKeyEvent *);
    void                    keyReleaseEvent         (QKeyEvent *);
    virtual void            showEvent               (QShowEvent *)                          override;
};

#endif // IPPROCESSGRID_H
