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

#include "IPProcessGrid.h"

IPProcessGrid::IPProcessGrid(QWidget *parent) : QGraphicsView(parent)
{
    _scene = new IPProcessGridScene(this);

    setScene(_scene);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    //setCacheMode(QGraphicsView::CacheBackground);
    centerOn(0,0);

    setDragMode(QGraphicsView::RubberBandDrag);
    setMouseTracking(true);

    _scale = 1.0;

    _isRunning = false;
    _isSequenceRunning = true;

    _currentStep = NULL;

    _stopExecution = false;

    _sequenceCount = 0;
    _sequenceIndex = 0;
    _lastSequenceIndex = 0;

    _lastProcessSuccess = false;

    _updateNeeded = true;

    _thread = NULL;

    // add a dummy object to allow correct placement of new objects with drag&drop
    scene()->addItem(new QGraphicsRectItem(0,0,0,0));

    connect(_scene, &QGraphicsScene::sceneRectChanged, this, &IPProcessGrid::sceneRectChanged);
}

bool IPProcessGrid::sortTreeDepthLessThan(IPProcessStep* s1, IPProcessStep* s2)
{
    return s1->treeDepth() < s2->treeDepth();
}

//breath first search
void IPProcessGrid::buildQueue()
{
    qDebug() << "IPProcessGrid::buildQueue";
    QQueue<IPProcessStep*> tmpQueue;
    _processList.clear();

    // find source processes
    int branchID = 0;
    for(auto it = _scene->steps()->begin(); it < _scene->steps()->end(); ++it)
    {
        IPProcessStep* step = (IPProcessStep*) *it;
        IPLProcess* process = step->process();

        // attach property changed event handler
        process->registerPropertyChangedEventHandler(this);
        process->registerOutputsChangedEventHandler(this);

        if(process->isSource())
        {
            step->setTreeDepth(0);
            step->setBranchID(branchID++);
            _processList.push_back(step);
            tmpQueue.enqueue(step);
        }
    }

    // add all other process steps with BFS
    int counter = 0;
    int limit   = 100;
    while(!tmpQueue.isEmpty() && counter < limit)
    {
        IPProcessStep* step = tmpQueue.dequeue();

        for(auto it = step->edgesOut()->begin(); it < step->edgesOut()->end(); ++it)
        {
            IPProcessEdge* edge = (IPProcessEdge*) *it;
            IPProcessStep* nextStep = edge->to();

            // set depth
            nextStep->setTreeDepth(step->treeDepth()+1);

            // set branch ID
            nextStep->setBranchID(step->branchID());

            // add to queue and list
            tmpQueue.enqueue(nextStep);

            // unique
            if(!_processList.contains(nextStep))
            {
                _processList.push_back(nextStep);
            }
        }
    }

    // sort by depth
    qSort(_processList.begin(), _processList.end(), IPProcessGrid::sortTreeDepthLessThan);

    // et voila, we have the execution order

    // move the tabs in the right order
    _mainWindow->imageViewer()->sortTabs();
}

int IPProcessGrid::executeThread(IPLProcess* process, IPLImage *image /*= NULL*/, int inputIndex /*= 0*/, bool useOpenCV /*= false*/)
{
    qDebug() << "IPProcessGrid::executeThread: " << QString::fromStdString(process->className());
    QElapsedTimer timer;
    timer.start();

    // create new thread
    _thread = new IPProcessThread(process, image, inputIndex, useOpenCV);

    connect(_thread, &IPProcessThread::progressUpdated, this, &IPProcessGrid::updateProgress);

    _mainWindow->setThreadRunning(true);
    _mainWindow->imageViewer()->zoomWidget()->zoomUpdateMutex()->lock();
    process->setResultReady(false);
    process->resetMessages();

    _thread->start();
    while(!_thread->isFinished())
    {
        if(_longProcess)
            _currentStep->update();

        QApplication::processEvents();
    }
    process->setResultReady(_thread->success());
    _mainWindow->setThreadRunning(false);
    _mainWindow->imageViewer()->zoomWidget()->zoomUpdateMutex()->unlock();

    _lastProcessSuccess = _thread->success();

    delete _thread;
    _thread = NULL;

    // return duration
    return timer.elapsed();
}

void IPProcessGrid::propagateNeedsUpdate(IPLProcess* process)
{
    qDebug() << "IPProcessGrid::propagateNeedsUpdate: " << QString::fromStdString(process->className());
    QQueue<IPProcessStep*> tmpQueue;

    // find step from process
    for(auto it = _scene->steps()->begin(); it < _scene->steps()->end(); ++it)
    {
        IPProcessStep* step = (IPProcessStep*) *it;
        IPLProcess* tmpProcess = step->process();

        if(tmpProcess == process)
        {
            step->process()->requestUpdate();

            tmpQueue.enqueue(step);
            break;
        }
    }


    // add all following processes via BFS
    int counter = 0;
    int limit   = 100;
    while(!tmpQueue.isEmpty() && counter < limit)
    {
        // set status
        IPProcessStep* step = tmpQueue.dequeue();

        for(auto it = step->edgesOut()->begin(); it < step->edgesOut()->end(); ++it)
        {
            IPProcessEdge* edge = (IPProcessEdge*) *it;
            IPProcessStep* nextStep = edge->to();

            nextStep->process()->requestUpdate();

            // add to queue and list
            tmpQueue.enqueue(nextStep);
        }
    }
}

void IPProcessGrid::propagateResultReady(IPLProcess* process, bool resultReady)
{
    qDebug() << "IPProcessGrid::propagateResultReady: " << QString::fromStdString(process->className()) << ", " << (resultReady ? "true" : "false");
    QQueue<IPProcessStep*> tmpQueue;

    // find step from process
    for(auto it = _scene->steps()->begin(); it < _scene->steps()->end(); ++it)
    {
        IPProcessStep* step = (IPProcessStep*) *it;
        IPLProcess* tmpProcess = step->process();

        if(tmpProcess == process)
        {
            step->process()->setResultReady(resultReady);

            tmpQueue.enqueue(step);
            break;
        }
    }


    // add all following processes via BFS
    int counter = 0;
    int limit   = 100;
    while(!tmpQueue.isEmpty() && counter < limit)
    {
        // set status
        IPProcessStep* step = tmpQueue.dequeue();

        for(auto it = step->edgesOut()->begin(); it < step->edgesOut()->end(); ++it)
        {
            IPProcessEdge* edge = (IPProcessEdge*) *it;
            IPProcessStep* nextStep = edge->to();

            nextStep->process()->setResultReady(resultReady);

            // add to queue and list
            tmpQueue.enqueue(nextStep);
        }
    }

    _mainWindow->imageViewer()->updateImage();
}

/*!
 * \brief IPProcessGrid::execute
 */
void IPProcessGrid::execute(bool forcedUpdate /* = false*/)
{
    // if no processes yet, then exit
    if(_scene->steps()->size() < 1)
    {
        return;
    }

    // if already running or nothing has changed, exit
    if(_isRunning || !_updateNeeded)
    {
        return;
    }
    // prevent user changes during execution
    _mainWindow->lockScene();
    _isRunning = true;
    _sequenceCount = 0;

    buildQueue();

    int totalDurationMs = 0;

    // execute the processes
    int counter = 0;
    int limit = 10000;
    bool blockFailLoop = false;

    QList<IPProcessStep*> afterProcessingList;

    QListIterator<IPProcessStep *> it(_processList);
    while (it.hasNext() && counter < limit)
    {
        if(_stopExecution)
            return;

        IPProcessStep* step = it.next();
        _currentStep = step;

        // make sure the progress bar gets filled
        updateProgress(1);

        // source processes don't have inputs
        if(step->process()->isSource())
        {
            // execute thread
            if(step->process()->updateNeeded() || forcedUpdate)
            {
                step->process()->resetMessages();
                step->process()->beforeProcessing();
                int durationMs = executeThread(step->process());
                if ( !_lastProcessSuccess ) blockFailLoop = true;

                // afterProcessing will be called later
                afterProcessingList.append(step);

                totalDurationMs += durationMs;
                step->setDuration(durationMs);

                // update error messages
                _mainWindow->updateProcessMessages();
            }
        }
        else
        {
            if(step->process()->updateNeeded() || forcedUpdate)
            {
                // execute process once for every input
                for(int i=0; i < step->edgesIn()->size(); i++)
                {
                    IPProcessEdge* edge = step->edgesIn()->at(i);
                    int indexFrom = edge->indexFrom();
                    int indexTo = edge->indexTo();
                    IPProcessStep* stepFrom = edge->from();

                    IPLImage* result = static_cast<IPLImage*>(stepFrom->process()->getResultData(indexFrom));

                    // invalid result, stopp the execution
                    if(!result)
                    {
                        QString msg("Invalid operation at step: ");
                        msg.append(QString::fromStdString(stepFrom->process()->title()));
                        _mainWindow->showMessage(msg, MainWindow::MESSAGE_ERROR);
                        break;
                    }

                    // execute thread
                    step->process()->resetMessages();
                    step->process()->beforeProcessing();
                    int durationMs = executeThread(step->process(), result, indexTo, mainWindow()->useOpenCV());
                    if ( !_lastProcessSuccess ) blockFailLoop = true;

                    // afterProcessing will be called later
                    afterProcessingList.append(step);

                    totalDurationMs += durationMs;
                    step->setDuration(durationMs);

                    // update error messages
                    _mainWindow->updateProcessMessages();
                }
            }
        }

        // make sure the progress bar gets filled
        updateProgress(100);

        counter++;
    }

    if(_stopExecution)
        return;

    // call afterProcessing of all steps which were executed this time
    // processes like the camera might request another execution
    QListIterator<IPProcessStep *> it2(_processList);
    while (it2.hasNext())
    {
        IPProcessStep* step = it2.next();
        step->process()->setUpdateNeeded(false);
    }
    QListIterator<IPProcessStep *> it3(_processList);
    while (it3.hasNext())
    {
        IPProcessStep* step = it3.next();
        step->updateThumbnail();
        step->process()->afterProcessing();
    }

    _updateNeeded = false;

    // check to see if any of these items changed while running,
    // set _updateNeeded to true if any still need it
    // this can happen if a slider is still being dragged after
    // a process is started
    // blockFailLoop prevents an infinite loop if a process is failing
    if ( !blockFailLoop ){
        QListIterator<IPProcessStep *> itp(_processList);
        while (itp.hasNext())
        {
            IPProcessStep* step = itp.next();
            if (step->process()->updateNeeded() )
            {
                _updateNeeded = true;
                break;
            }
        }
    }

    // update images
    _mainWindow->imageViewer()->updateImage();
    _mainWindow->imageViewer()->showProcessDuration(totalDurationMs);

    // update process graph
    _mainWindow->updateGraphicsView();
    _mainWindow->unlockScene();

    _isRunning = false;
    _currentStep = NULL;
}

void IPProcessGrid::terminate()
{
    qDebug() << "IPProcessGrid::terminate";
    if(_thread)
        _thread->terminate();
}

void IPProcessGrid::updateProgress(int progress)
{
    // enable spinning progress for long operations
    _longProcess = (progress < 0);

    if(_currentStep)
    {
        _currentStep->setProgress(progress);
    }
}


void IPProcessGrid::sceneRectChanged(const QRectF &rect)
{
    fitLargeSceneRect();
}

/*!
 * \brief IPProcessGrid::zoomIn
 */
void IPProcessGrid::zoomIn()
{
    zoomBy(0.1f);
}

/*!
 * \brief IPProcessGrid::zoomOut
 */
void IPProcessGrid::zoomOut()
{
    zoomBy(-0.1f);
}

/*!
 * \brief IPProcessGrid::zoomBy
 * \param scaleChange
 */
void IPProcessGrid::zoomBy(float scaleChange)
{
    zoomTo(_scale + scaleChange);
}

/*!
 * \brief IPProcessGrid::zoomTo
 * \param newScale
 */
void IPProcessGrid::zoomTo(float newScale)
{
    newScale = newScale > 3.0 ? 3.0 : newScale;
    newScale = newScale < 0.5 ? 0.5 : newScale;

    _scale = newScale;

    QMatrix matrix;
    matrix.scale(_scale, _scale);
    setMatrix(matrix);

    // update statusbar
    _mainWindow->statusBar()->showMessage(QString("Zoom: %1%").arg(QString::number(_scale*100, 'f', 0)));
}

/*!
 * \brief IPProcessGrid::wheelEvent
 * \param event
 */
void IPProcessGrid::wheelEvent(QWheelEvent* event)
{
#ifdef Q_OS_MACX
    // Qt handles mac trackpads and magic mouse events very badly,
    // it is better to just ignore them for now...
    return;
#endif
    if(event->angleDelta().y() > 0)
        zoomIn();
    else
        zoomOut();
}

/*!
 * \brief IPProcessGrid::showEvent
 */
void IPProcessGrid::showEvent(QShowEvent *e)
{
    //set the scene rect to allow more space when zooming
    if ( !e->spontaneous() ){
       fitLargeSceneRect();
    }
}

/*!
 * \brief IPProcessGrid::resizeEvent
 */
void IPProcessGrid::resizeEvent(QResizeEvent *)
{
   fitLargeSceneRect();
}

/*!
 * \brief IPProcessGrid::fitLargeSceneRect
 */
void IPProcessGrid::fitLargeSceneRect()
{
    qreal width = this->width()*2;
    qreal height = this->height()*2;
    qreal x = 0;
    qreal y = 0;
    if ( scene()->sceneRect().width() > width )
        width = scene()->sceneRect().width();
    if ( scene()->sceneRect().height() > height )
        height = scene()->sceneRect().height();
    if ( scene()->sceneRect().x() < x )
        x = scene()->sceneRect().x();
    if ( scene()->sceneRect().y() < y )
        y = scene()->sceneRect().y();

    setSceneRect(x,y,width,height);
}

/*!
 * \brief IPProcessGrid::keyPressEvent
 * \param event
 */
void IPProcessGrid::keyPressEvent(QKeyEvent *event)
{
    Qt::KeyboardModifiers modifiers = event->modifiers();

    if(event->key() == Qt::Key_Space)
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
    }

    if(event->key() == Qt::Key_F && modifiers&Qt::ControlModifier)
    {
        _mainWindow->hideProcessSettings();
        _mainWindow->setFilterFocus();
    }

    if(event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)
    {
        _scene->deleteSelectedItems();
    }

    if(event->key() == Qt::Key_Escape)
    {
        _mainWindow->hideProcessSettings();
    }
}

/*!
 * \brief IPProcessGrid::keyReleaseEvent
 * \param event
 */
void IPProcessGrid::keyReleaseEvent(QKeyEvent* event)
{
    Qt::KeyboardModifiers modifiers = QGuiApplication::keyboardModifiers();

    if(event->key() == Qt::Key_Space)
    {
        setDragMode(QGraphicsView::RubberBandDrag);
    }
}



void IPProcessGrid::propertyChanged(IPLProcess* process)
{
    //process->requestUpdate();
    propagateNeedsUpdate(process);
    _updateNeeded = true;

    _mainWindow->updateProcessMessages();
}

void IPProcessGrid::outputsChanged(IPLProcess *)
{
    _mainWindow->imageViewer()->updateOutputs();
}

void IPProcessGrid::setSequenceIndex(int index)
{
    _lastSequenceIndex = _sequenceIndex;
    _sequenceIndex = index;
}

void IPProcessGrid::requestUpdate()
{
    _updateNeeded = true;
}

