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

#include "IPProcessGridScene.h"

IPProcessGridScene::IPProcessGridScene(QObject *parent) :
    QGraphicsScene(parent)
{
    QBrush backgroundBrush(QImage(":/blueprint.png"));
    setBackgroundBrush(backgroundBrush);

    // add temporary arrow item
    _tmpArrow = new IPProcessEdgeTmp;
    addItem(_tmpArrow);

    _showThumbnails = false;
}

IPProcessGridScene::~IPProcessGridScene()
{
    foreach (IPProcessEdge* e, _edges) {
        delete e;
    }
    foreach (IPProcessStep* s, _steps) {
        delete s;
    }
}

void IPProcessGridScene::snapSelectedItemsToGrid()
{
    for(int i=0; i < selectedItems().length(); i++)
    {
        IPProcessStep* item = (IPProcessStep*)selectedItems().at(i);
        item->snapToGrid();
    }
}

void IPProcessGridScene::deleteSelectedItems()
{
    // close the settings window to prevent invalid pointers
    ((IPProcessGrid*) parent())->mainWindow()->hideProcessSettings();
    ((IPProcessGrid*) parent())->mainWindow()->setAllowChangeActiveProcessStep(false);

    QList<QGraphicsItem*> selected = selectedItems();

    // remove the items
    while(!selected.empty())
    {
        // only take selected steps, not other items
        if(typeid(*selected.first()) == typeid(IPProcessStep))
        {
            IPProcessStep* step = (IPProcessStep*) selected.first();
            removeStep(step);
        }
        selected.removeFirst();
    }

    ((IPProcessGrid*) parent())->mainWindow()->setActiveProcessStep(NULL);
    ((IPProcessGrid*) parent())->mainWindow()->setAllowChangeActiveProcessStep(true);
}

void IPProcessGridScene::addStep(IPProcessStep* step)
{
    _steps.append(step);
    addItem(step);
}

void IPProcessGridScene::removeStep(IPProcessStep* step)
{
    ((IPProcessGrid*) parent())->mainWindow()->setAllowChangeActiveProcessStep(false);

    // remove from image viewer window
    ((IPProcessGrid*) parent())->mainWindow()->imageViewer()->removeProcessStep(step);

    // remove edges
    while(!step->edgesIn()->empty())
    {
        IPProcessEdge* edge = step->edgesIn()->first();
        removeEdge(edge);
    }

    while(!step->edgesOut()->empty())
    {
        IPProcessEdge* edge = step->edgesOut()->first();
        removeEdge(edge);
    }

    // remove step
    _steps.removeAll(step);
    delete step;

    ((IPProcessGrid*) parent())->mainWindow()->setAllowChangeActiveProcessStep(true);
}

bool IPProcessGridScene::addEdge(IPProcessEdge* edge)
{
    if(edge->from() == edge->to())
        return false;

    // check type
    int indexOut = edge->indexFrom();
    int indexIn  = edge->indexTo();
    IPLProcessIO output = edge->from()->process()->outputs()->at(indexOut);
    IPLProcessIO input  = edge->to()->process()->inputs()->at(indexIn);

    // inputs can accept lower types
    // COLOR accepts GRAY and BW
    if(output.type > input.type)
        return false;

    /*IPLData* outputData = edge->from()->process()->getResultData(indexOut);
    if (!outputData->isConvertibleTo(input.type))
        return false;*/

    _edges.append(edge);
    addItem(edge);

    //set outgoing and ingoing edges
    edge->from()->addEdgeOut(edge);
    edge->to()->addEdgeIn(edge);

    // set input occupied
    edge->to()->process()->inputs()->at(edge->indexTo()).occupied = true;

    return true;
}

void IPProcessGridScene::removeEdge(IPProcessEdge* edge)
{
    // set inputs not occupied
    edge->to()->process()->inputs()->at(edge->indexTo()).occupied = false;

    edge->from()->removeEdgeOut(edge);
    edge->to()->removeEdgeIn(edge);

    _edges.removeAll(edge);
    delete edge;
}


void IPProcessGridScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    // only accept process steps or files from the OS
    if(event->mimeData()->hasFormat("application/x-imageplay") || event->mimeData()->hasUrls())
        event->setAccepted(true);
    else
        event->setAccepted(false);
}

void IPProcessGridScene::dragMoveEvent(QGraphicsSceneDragDropEvent*)
{
}

void IPProcessGridScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    QString processID = event->mimeData()->data("application/x-imageplay");

    // check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        QMimeDatabase db;

        // extract the local paths of the files
        QPointF offset;
        for (int i = 0; i < urlList.size() && i < 32; i++)
        {
            QString filePath = urlList.at(i).toLocalFile();

            QMimeType type = db.mimeTypeForFile(filePath);

            // automatically load IPJ files
            if(type.name() == "text/plain" && filePath.endsWith(".ipj"))
            {
                MainWindow* mainWindow = ((IPProcessGrid*) parent())->mainWindow();
                if(mainWindow)
                {
                    mainWindow->readProcessFile(filePath);
                }

                return;
            }

            // automatically add IPLLoadImage for image types
            if(type.name().startsWith("image/"))
            {
                IPProcessStep* newStep = createProcessStep("IPLLoadImage", event->scenePos() + offset);
                IPLLoadImage* stepLoadImage = dynamic_cast<IPLLoadImage*>(newStep->process());

                if(stepLoadImage)
                {
                    stepLoadImage->setPath(filePath.toStdString());
                }
            }

            // automatically add IPLLoadImageSequence for folders
            if(type.name() == "inode/directory")
            {
                IPProcessStep* newStep = createProcessStep("IPLLoadImageSequence", event->scenePos() + offset);
                IPLLoadImageSequence* stepLoadImageSequence = dynamic_cast<IPLLoadImageSequence*>(newStep->process());

                if(stepLoadImageSequence)
                {
                    stepLoadImageSequence->setFolder(filePath.toStdString());
                }
            }

            offset += QPointF(0, 50);
        }

        // call a function to open the files
        //openFiles(pathList);
    }

    createProcessStep(processID, event->scenePos());
}

bool IPProcessGridScene::isSupportedMimeType(const QMimeData* mimeData)
{
    // check for image/* mime types
    foreach(QString type, mimeData->formats())
    {
        if(type.startsWith("image/"))
            return true;
    }
    /*std::vector<std::string> supportedMimeTypes = IPLFileIO::supportedMimeTypes();

    // check for supported image file types
    foreach(std::string type, supportedMimeTypes)
    {
        if(mimeData->hasFormat(QString::fromStdString(type)))
            return true;
    }*/

    // check for imageplay process

    if(mimeData->hasFormat("application/x-imageplay"))
        return true;

    return false;
}

IPProcessStep* IPProcessGridScene::createProcessStep(QString processID, QPointF scenePos)
{
    if(processID.length() == 0)
        return NULL;

    MainWindow* mainWindow = ((IPProcessGrid*) parent())->mainWindow();

    // create process step
    IPProcessStep* newProcessStep = new IPProcessStep(mainWindow, processID); //(this);

    // calculate relative position
    QPointF pos = scenePos - QPointF(newProcessStep->boundingRect().width()/2, newProcessStep->boundingRect().height()/2);
    newProcessStep->setPos(pos);
    newProcessStep->snapToGrid();

    mainWindow->addStep(newProcessStep);

    return newProcessStep;
}
