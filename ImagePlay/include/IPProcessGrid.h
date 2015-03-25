//#############################################################################
//  File:      IPProcessGrid.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
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
class IPProcessGrid : public QGraphicsView, public IPLPropertyChangedEventHandler
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
    int                     executeThread           (IPLProcess* process, IPLImage *image, int inputIndex, bool useOpenCV = false);
    void                    propagateNeedsUpdate    (IPLProcess* process);
    void                    propertyChanged         (IPLProcess *);
    void                    setSequenceIndex        (int index);
    void                    setSequenceRunning      (bool status)                           { _isSequenceRunning = status; }
    void                    setMainWindow           (MainWindow* mainWindow)                { _mainWindow = mainWindow; }
    void                    requestUpdate           ();
    MainWindow*             mainWindow              ()                                      { return _mainWindow; }
    IPProcessGridScene*     scene                   ()                                      { return _scene; }

signals:
    void                    sequenceChanged         (int index, int count);

public slots:
    void                    execute                 (bool forcedUpdate = false);
    void                    updateProgress          (int);

private:
    IPProcessGridScene*     _scene;                 //!< Scene
    float                   _scale;                 //!< Scale for zooming
    MainWindow*             _mainWindow;            //!< MainWindow
    bool                    _isRunning;             //!< Is running
    IPProcessStep*          _currentStep;           //!< Currently active step, settings shown on the left side
    QList<IPProcessStep*>   _processList;           //!< Ordered process list
    int                     _sequenceCount;         //!< Image sequence count
    int                     _sequenceIndex;         //!< Current image sequence index
    int                     _lastSequenceIndex;     //!< Last image sequence index
    bool                    _isSequenceRunning;     //!< Is sequence running
    bool                    _lastProcessSuccess;    //!< Last process was successful
    long long               _currentUpdateID;       //!< Used to check if an update is needed
    long long               _updateID;              //!<

    // QWidget interface
protected:
    void                    wheelEvent              (QWheelEvent *);
    void                    showEvent               (QShowEvent *);
    void                    resizeEvent             (QResizeEvent *);
    void                    keyPressEvent           (QKeyEvent *);
    void                    keyReleaseEvent         (QKeyEvent *);
};

#endif // IPPROCESSGRID_H
