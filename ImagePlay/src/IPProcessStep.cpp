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

#include "IPProcessStep.h"

#include "IPProcessGridScene.h"

// init step counter for automatic step ID
long IPProcessStep::_stepCounter = 0;

/*!
 * \brief IPProcessStep::IPProcessStep
 * \param mainWindow
 * \param processName
 */
IPProcessStep::IPProcessStep(MainWindow* mainWindow, QString processID)
{
    // increment step ID
    _stepID = ++_stepCounter;

    _durationMs = 0;
    _progress = 0;
    _branchID = 0;
    _treeDepth = 0;

    _progressFrame = 0;

    // set QGraphicItem properties
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    _editing = false;
    _connecting = false;

    setCursor(Qt::SizeAllCursor);

    _mainWindow = mainWindow;

    // try to create an IPL process
    _process = _mainWindow->factory()->getInstance(processID);

    if(_process == NULL)
    {
        setName("INVALID PROCESS");
    }
    else
    {
        setName(processID);
    }


    // try to find icon
    QFileInfo iconFile(_mainWindow->processIconPath(processID));
    if(iconFile.isFile())
    {
        setIcon(QIcon(iconFile.absoluteFilePath()));
    }
}

IPProcessStep::~IPProcessStep()
{
    delete _process;
}

/*!
 * \brief IPProcessStep::boundingRect
 * \return
 */
QRectF IPProcessStep::boundingRect() const
{
    /// @todo add real bounding rect to prevent false clipping!
    return QRectF(7,7,50,50);
}

/*!
 * \brief IPProcessStep::paint
 * \param painter
 * \param option
 * \param widget
 */
void IPProcessStep::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    // shadow
    painter->fillRect(QRect(10,10,48,48), QColor(0,0,0,50));

    // pen
    QPen pen(QColor(0,0,0));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    if(isSelected())
    {
        pen.setColor(QColor(52, 152, 219));
    }
    if(isEditing())
    {
        pen.setColor(QColor(211, 84, 0));
    }
    if(!process()->isResultReady())
    {
        pen.setStyle(Qt::DotLine);
    }
    painter->setPen(pen);

    // brush
    QBrush brush(Qt::white);
    painter->setBrush(brush);

    // inputs
    /*int input_y = 0;
    for(int i=0; i < (int) process()->inputs()->size(); i++)
    {
        // check if output is free
        if(!process()->inputs()->at(i).occupied)
        {
            painter->drawEllipse(0, input_y, 10, 10);
            input_y += 20;
        }
    }

    // outputs
    int output_y = 0;
    for(int i=0; i < (int) process()->outputs()->size(); i++)
    {
        // check if output is free
        if(!process()->outputs()->at(i).occupied)
        {
            painter->drawEllipse(55, output_y, 10, 10);
            input_y += 20;
        }
    }*/

    // main box
    QRect rect(8,8,48,48);
    //painter->fillRect(rect);
    painter->drawRect(rect);

    // THUMBNAIL MODE
    if(((IPProcessGridScene*) scene())->showThumbnails() && _thumbnail.height() > 0)
    {
        painter->drawPixmap(9,9,46,46,_thumbnail);

        // only show progress if not yet finished
        if(_progress > 0 && _progress < 100)
        {
            // progress bar
            int progressWidth = _progress*46/100;
            QBrush brushWhite(Qt::white);
            QBrush brushGreen(QColor(39, 174, 96));
            painter->fillRect(QRect(9,42,46,14), brushWhite);
            painter->fillRect(QRect(9,42,progressWidth,14), brushGreen);

            // text
            QFont fontSmallBlack(painter->font());
            fontSmallBlack.setPointSize(7);
            fontSmallBlack.setBold(false);
            QRect textBounds2(8,44,48,14);
            QPen penBlack(Qt::black);
            painter->setPen(penBlack);
            painter->setFont(fontSmallBlack);
            painter->drawText(textBounds2, Qt::AlignRight, QString("%1%").arg(_progress));
        }
    }
    else
    {

        // separator
        painter->drawLine(8,40,56,40);

        // main text
        QRect textBounds(9,9,46,32);
        QFont fontBold(painter->font());
        fontBold.setBold(false);
        fontBold.setPixelSize(9);
        painter->setPen(QPen(Qt::black));
        painter->setFont(fontBold);

        QString title = QString::fromStdString(_process->title());
        painter->drawText(textBounds, Qt::AlignCenter | Qt::TextWordWrap, title);


        // only show progress if not yet finished
        if(_progress > 0 && _progress < 100)
        {
            // progress bar
            int progressWidth = _progress*32/100;
            QBrush brushGreen(QColor(39, 174, 96));
            painter->fillRect(QRect(23,41,progressWidth,14), brushGreen);

            // text
            QFont fontSmallBlack(painter->font());
            fontSmallBlack.setPointSize(7);
            fontSmallBlack.setBold(false);
            QRect textBounds2(22,43,32,14);
            QPen penBlack(Qt::black);
            painter->setPen(penBlack);
            painter->setFont(fontSmallBlack);
            painter->drawText(textBounds2, Qt::AlignRight, QString("%1%").arg(_progress));
        }
        else if(_progress < 0)
        {
            // draw rotating progress for unmeasurable operations
            QBrush brushGreen(QColor(39, 174, 96));

            QPolygon parallelogram;
            parallelogram << QPoint(7,0) << QPoint(14,0) << QPoint(7,14) << QPoint(0,14);

            // enable clipping
            painter->setClipRect(9,41,46,14);

            for(int i=0; i<8; i++)
            {
                QPainterPath path;
                path.addPolygon(parallelogram);
                path.translate(_progressFrame+i*14-10,42);
                painter->fillPath(path, brushGreen);
            }

            // disable clipping
            painter->setClipping(false);

            // count 0-14
            _progressFrame++;
            _progressFrame = (_progressFrame > 14) ? 0 : _progressFrame;
        }
        else
        {
            // footer text
            QFont fontSmallGray(painter->font());
            fontSmallGray.setPointSize(7);
            fontSmallGray.setBold(false);
            QRect textBounds2(22,43,32,14);
            QPen penGray(Qt::gray);
            painter->setPen(penGray);
            painter->setFont(fontSmallGray);
            painter->drawText(textBounds2, Qt::AlignRight, QString("%1ms").arg(_durationMs));
        }


        // icon
        //_icon.paint(painter,9,41,14,14);

    }

    // Errors or warnings
    if(process()->hasErrors())
    {
        QFont fontSmallGray(painter->font());
        fontSmallGray.setBold(true);

        QRect idBounds(5,5,34,10);
        QBrush brushRed(QColor(200,0,0));
        QPen penRed(QColor(50,0,0));
        painter->setBrush(brushRed);
        painter->setPen(penRed);
        painter->fillRect(idBounds, brushRed);
        painter->drawRect(idBounds);

        QPen penWhite(QColor(255,255,255));
        painter->setPen(penWhite);
        fontSmallGray.setPointSize(6);
        painter->setFont(fontSmallGray);
        //painter->drawText(idBounds, Qt::AlignCenter, QString::number(stepID()));
        painter->drawText(idBounds, Qt::AlignCenter, "ERROR");
    }
    else if(process()->hasWarnings())
    {
        QFont fontSmallGray(painter->font());
        fontSmallGray.setBold(true);

        QRect idBounds(5,5,44,10);
        QBrush brushYellow(QColor(252,248,227));
        QPen penYellow(QColor(138,109,59));
        painter->setBrush(brushYellow);
        painter->setPen(penYellow);
        painter->fillRect(idBounds, brushYellow);
        painter->drawRect(idBounds);

        fontSmallGray.setPointSize(6);
        painter->setFont(fontSmallGray);
        //painter->drawText(idBounds, Qt::AlignCenter, QString::number(stepID()));
        painter->drawText(idBounds, Qt::AlignCenter, "WARNING");
    }

    // OpenCV badge
    if(_process->openCVSupport() == IPLProcess::OPENCV_ONLY || (_process->openCVSupport() == IPLProcess::OPENCV_OPTIONAL  && _mainWindow->useOpenCV()))
    {
        QFont fontSmallGray(painter->font());
        fontSmallGray.setBold(true);

        QRect bounds(5, 50, 10, 10);
        QBrush brushGreen(QColor(0,200,0));
        QPen penGreen(QColor(0,50,0));
        painter->setBrush(brushGreen);
        painter->setPen(penGreen);
        painter->fillRect(bounds, brushGreen);
        painter->drawRect(bounds);

        QPen penWhite(QColor(255,255,255));
        painter->setPen(penWhite);
        fontSmallGray.setPointSize(6);
        painter->setFont(fontSmallGray);
        painter->drawText(bounds, Qt::AlignCenter, "CV");
    }
}

/*!
 * \brief IPProcessStep::snapToGrid
 */
void IPProcessStep::snapToGrid()
{
    int x = pos().x();
    int y = pos().y();

    QPoint pos(x, y);

    // snapt to nearest grid position
    QPointF newPos = (pos / 64)*64;
    setPos(newPos);

    // update edges
    foreach(IPProcessEdge* edge, _edgesIn)
    {
        edge->updatePosition();
    }

    foreach(IPProcessEdge* edge, _edgesOut)
    {
        edge->updatePosition();
    }
}

void IPProcessStep::removeEdgeIn(IPProcessEdge *edge)
{
    _edgesIn.removeAll(edge);
    _mainWindow->propagateResultReady(this->process(), false);
    setTreeDepth(-1);
}

void IPProcessStep::updateThumbnail()
{
    if(process()->isResultReady())
    {
        IPLData* data = process()->getResultData(0);
        IPLImage* image = data->toImage();
        if(image)
        {
            QImage tmpImage = QImage(image->rgb32(), image->width(), image->height(), QImage::Format_RGB32);
            _thumbnail = QPixmap::fromImage(tmpImage.scaled(138,138, Qt::KeepAspectRatioByExpanding, Qt::FastTransformation));
        }
    }
}

void IPProcessStep::setProgress(int progress)
{
    _progress = progress > 100 ? 100 : progress;

    // redraw
    update(boundingRect());
}

/*!
 * \brief IPProcessStep::mousePressEvent
 * \param event
 */
void IPProcessStep::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        _connecting = true;
        ((IPProcessGridScene*) scene())->setTmpArrowFrom(this->pos());
        return;
    }

    // bring the item to the front
    setZValue(1000);
    if(scene()->selectedItems().length() > 1)
    {
        for(int i=0; i < scene()->selectedItems().length(); i++)
        {
            IPProcessStep* item = (IPProcessStep*) scene()->selectedItems().at(i);
            item->setZValue(1000);
        }
    }

    // save last position
    _lastPosition = pos();

    if(scene()->selectedItems().length() > 1)
    {
        for(int i=0; i < scene()->selectedItems().length(); i++)
        {
            IPProcessStep* item = (IPProcessStep*) scene()->selectedItems().at(i);
            item->_lastPosition = item->pos();
        }
    }

    QGraphicsItem::mousePressEvent(event);
}

/*!
 * \brief IPProcessStep::mouseReleaseEvent
 * \param event
 */
void IPProcessStep::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    IPProcessGridScene* scn = (IPProcessGridScene*) scene();

    // bring the items to the background again
    setZValue(0);
    if(scene()->selectedItems().length() > 1)
    {
        for(int i=0; i < scene()->selectedItems().length(); i++)
        {
            IPProcessStep* item = (IPProcessStep*) scene()->selectedItems().at(i);
            item->setZValue(0);
        }
    }

    // check for items for connection
    if(_connecting)
    {
        // there is currently only 1 view
        QGraphicsView* view = scn->views()[0];
        // transform is needed because of zooming capabilities
        QGraphicsItem* tmpItem = scn->itemAt(event->scenePos(), view->transform());
        if(tmpItem && typeid(*tmpItem) == typeid(IPProcessStep))
        {
            IPProcessStep* otherItem = (IPProcessStep*) tmpItem;
            if(process()->availableOutputs() > 0 && otherItem->process()->availableInputs() > 0)
            {

                // if source or destination have multiple inputs/outputs show connection dialog
                if(process()->outputs()->size() > 1 || otherItem->process()->inputs()->size() > 1)
                {
                    ConnectionDialog* connectionDialog = new ConnectionDialog(this, otherItem, scn, _mainWindow);
                    connectionDialog->show();
                }
                else
                {
                    _mainWindow->addEdge(new IPProcessEdge(this, otherItem, 0, 0));
                }
            }

        }
        _connecting = false;

        ((IPProcessGridScene*) scene())->setTmpArrowVisible(false);
        return;
    }

    // snap after dragging
    snapToGrid();

    // snap all if multiselect
    bool groupIsColliding = false;
    if(scene()->selectedItems().length() > 0)
    {
        for(int i=0; i < scene()->selectedItems().length(); i++)
        {
            IPProcessStep* item = (IPProcessStep*) scene()->selectedItems().at(i);
            for(QGraphicsItem* currentItem : item->collidingItems())
            {
                IPProcessStep* step = dynamic_cast<IPProcessStep*>(currentItem);
                if(step)
                {
                    groupIsColliding = true;
                    break;
                }
            }
        }
    }
    if(groupIsColliding)
    {
        for(int i=0; i < scene()->selectedItems().length(); i++)
        {
            IPProcessStep* item = (IPProcessStep*) scene()->selectedItems().at(i);
            item->setPos(item->_lastPosition);
        }
    }

    for(int i=0; i < scene()->selectedItems().length(); i++)
    {
        IPProcessStep* item = (IPProcessStep*) scene()->selectedItems().at(i);
        item->snapToGrid();
    }

    setCursor(Qt::SizeAllCursor);

    QGraphicsItem::mouseReleaseEvent(event);
}

/*!
 * \brief IPProcessStep::mouseMoveEvent
 * \param event
 */
void IPProcessStep::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(_connecting)
    {
        // update temporary arrow position
        ((IPProcessGridScene*) scene())->setTmpArrowTo(event->scenePos());
        ((IPProcessGridScene*) scene())->setTmpArrowVisible(true);
    }


    // notify the scene that the geometry has changed
    //prepareGeometryChange();

    // if colliding with other item, change cursor
    bool groupIsColliding = false;
    if(scene()->selectedItems().length() > 0)
    {
        for(int i=0; i < scene()->selectedItems().length(); i++)
        {
            IPProcessStep* item = (IPProcessStep*) scene()->selectedItems().at(i);
            for(QGraphicsItem* currentItem : item->collidingItems())
            {
                IPProcessStep* step = dynamic_cast<IPProcessStep*>(currentItem);
                if(step)
                {
                    groupIsColliding = true;
                    break;
                }
            }
        }
    }
    if(groupIsColliding)
    {
        setCursor(Qt::ForbiddenCursor);
    }
    else
    {
        setCursor(Qt::SizeAllCursor);
    }

    QGraphicsItem::mouseMoveEvent(event);
}

/*!
 * \brief IPProcessStep::mouseDoubleClickEvent
 * \param event
 */
void IPProcessStep::mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)
{
    _mainWindow->setActiveProcessStep(this);
}


QVariant IPProcessStep::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionChange)
    {
        foreach(IPProcessEdge* edge, _edgesIn)
        {
            edge->updatePosition();
        }

        foreach(IPProcessEdge* edge, _edgesOut)
        {
            edge->updatePosition();
        }

    }
    return QGraphicsItem::itemChange(change, value);
}
