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
    _currentUpdateID = 0;
    _updateID = 0;

    _isRunning = false;
    _isSequenceRunning = true;

    _currentStep = NULL;

    _stopExecution = false;

    _sequenceCount = 0;
    _sequenceIndex = 0;
    _lastSequenceIndex = 0;

    _lastProcessSuccess = false;

    // add a dummy object to allow correct placement of new objects with drag&drop
    scene()->addItem(new QGraphicsRectItem(0,0,0,0));
}

bool IPProcessGrid::sortTreeDepthLessThan(IPProcessStep* s1, IPProcessStep* s2)
{
    return s1->treeDepth() < s2->treeDepth();
}

void IPProcessGrid::buildQueue()
{
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
            int depth = std::max(nextStep->treeDepth(), step->treeDepth()+1);
            nextStep->setTreeDepth(depth);

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
    /*qDebug() << "Process List:";
    QListIterator<IPProcessStep *> i(_processList);
    while (i.hasNext())
    {
        IPProcessStep* step = i.next();
        qDebug() << step->name() << "\tID: " << step->stepID() << "\tDepth: " << step->treeDepth();
    }*/

    // move the tabs in the right order
    _mainWindow->imageViewer()->sortTabs();
}

int IPProcessGrid::executeThread(IPLProcess* process, IPLImage *image = NULL, int inputIndex = 0, bool useOpenCV /*= false*/)
{
    QElapsedTimer timer;
    timer.start();

    // create new thread
    IPProcessThread thread(process, image, inputIndex, useOpenCV);
    connect(&thread, &IPProcessThread::progressUpdated, this, &IPProcessGrid::updateProgress);

    _mainWindow->setThreadRunning(true);
    process->setResultReady(false);
    thread.start();
    while(!thread.isFinished())
    {
        QApplication::processEvents();
    }
    process->setResultReady(thread.success());
    _mainWindow->setThreadRunning(false);

    _lastProcessSuccess = thread.success();

    // return duration
    return timer.elapsed();
}

void IPProcessGrid::propagateNeedsUpdate(IPLProcess* process)
{
    QQueue<IPProcessStep*> tmpQueue;

    // find step from process
    for(auto it = _scene->steps()->begin(); it < _scene->steps()->end(); ++it)
    {
        IPProcessStep* step = (IPProcessStep*) *it;
        IPLProcess* tmpProcess = step->process();

        if(tmpProcess == process)
        {
            step->process()->setResultReady(false);
            step->process()->setNeedsUpdate(true);

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

            nextStep->process()->setNeedsUpdate(step->process()->needsUpdate());

            // add to queue and list
            tmpQueue.enqueue(nextStep);
        }
    }
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
    if(_isRunning || (_currentUpdateID == _updateID))
    {
        return;
    }
    // prevent user changes during execution
    _mainWindow->lockScene();
    _isRunning = true;
    _sequenceCount = 0;
    _currentUpdateID = _updateID;

    buildQueue();

    int totalDurationMs = 0;

    // execute the processes
    int counter = 0;
    int limit = 100;

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
            // check if is sequence
            IPLLoadImageSequence* sequenceProcess = dynamic_cast<IPLLoadImageSequence*>(step->process());

            // update if index has changed
            if(sequenceProcess && (_sequenceIndex != _lastSequenceIndex))
            {
                sequenceProcess->setSequenceIndex(_sequenceIndex);
                propagateNeedsUpdate(sequenceProcess);
            }

            // execute thread
            if(step->process()->needsUpdate() || forcedUpdate)
            {
                int durationMs = executeThread(step->process());

                totalDurationMs += durationMs;
                step->setDuration(durationMs);

                if(step->process()->isResultReady())
                {
                    step->process()->setNeedsUpdate(false);

                    if(!step->process()->hasErrors())
                        step->updateThumbnail();
                }

                // update error messages
                _mainWindow->updateProcessMessages();
            }

            if(sequenceProcess)
            {
                int currentSequenceCount = sequenceProcess->sequenceCount();
                _sequenceCount = std::max(_sequenceCount, currentSequenceCount);
                emit sequenceChanged(_sequenceIndex, _sequenceCount);
            }
        }
        else
        {
            if(step->process()->needsUpdate() || forcedUpdate)
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
                    int durationMs = executeThread(step->process(), result, indexTo, mainWindow()->useOpenCV());
                    totalDurationMs += durationMs;
                    step->setDuration(durationMs);

                    if(step->process()->isResultReady())
                    {
                        step->process()->setNeedsUpdate(false);
                        step->updateThumbnail();
                    }

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

    // update images
    _mainWindow->imageViewer()->updateImage();
    _mainWindow->imageViewer()->showProcessDuration(totalDurationMs);


    // update process graph
    _mainWindow->updateGraphicsView();
    _mainWindow->unlockScene();

    _isRunning = false;
    _currentStep = NULL;

    // if sequence, then run execute next step
    if(_sequenceCount > 0)
    {
        // notify GUI
        emit sequenceChanged(_sequenceIndex, _sequenceCount);

        if(_isSequenceRunning)
        {
            //setParamsHaveChanged();
            _mainWindow->execute(true);
        }
    }

    // find sequence processes
    bool graphNeedsUpdate = false;
    for(auto it = _scene->steps()->begin(); it < _scene->steps()->end(); ++it)
    {
        IPProcessStep* step = (IPProcessStep*) *it;
        IPLProcess* process = step->process();

        if(process->isSequence())
        {
            process->setNeedsUpdate(true);
            propertyChanged(process);
            requestUpdate();
        }
    }

    if(_updateID > _currentUpdateID)
        _mainWindow->execute(false);

    _updateID = _currentUpdateID;

    // only for testing the camera
    if(graphNeedsUpdate)
        _mainWindow->execute(false);
}

void IPProcessGrid::updateProgress(int progress)
{
    if(_currentStep)
    {
        _currentStep->setProgress(progress);
    }
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
    if(event->angleDelta().y() > 0)
        zoomIn();
    else
        zoomOut();
}

/*!
 * \brief IPProcessGrid::showEvent
 */
void IPProcessGrid::showEvent(QShowEvent *)
{
    // add a big object to make sure the coordinate system starts top left
    //QPen invisiblePen(QColor(0,0,0));
    //_scene->addRect(0,0,width()-5,height()-5,invisiblePen);
}

/*!
 * \brief IPProcessGrid::resizeEvent
 */
void IPProcessGrid::resizeEvent(QResizeEvent *)
{
}

/*!
 * \brief IPProcessGrid::keyPressEvent
 * \param event
 */
void IPProcessGrid::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
}

/*!
 * \brief IPProcessGrid::keyReleaseEvent
 * \param event
 */
void IPProcessGrid::keyReleaseEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Space)
    {
        setDragMode(QGraphicsView::RubberBandDrag);
    }

    if(event->key() == Qt::Key_Delete)
    {
        _scene->deleteSelectedItems();
    }
    if(event->key() == Qt::Key_Escape)
    {
        _mainWindow->hideProcessSettings();
    }
}


void IPProcessGrid::propertyChanged(IPLProcess* process)
{
    propagateNeedsUpdate(process);
}

void IPProcessGrid::setSequenceIndex(int index)
{
    _lastSequenceIndex = _sequenceIndex;
    _sequenceIndex = index;
}

void IPProcessGrid::requestUpdate()
{
    _updateID++;
}
