#include "IPImageViewer.h"

IPImageViewer::IPImageViewer(ImageViewerWindow* imageViewer, QWidget *parent) :
    QFrame(parent)
{
    setLayout(new QHBoxLayout(this));

    _imageViewerWindow = imageViewer;

    //delete on close
    setAttribute(Qt::WA_DeleteOnClose);

    //layout()->setContentsMargins(0,0,0,0);

    _processStep = NULL;
    _image = NULL;
    _rawData = NULL;
    _rawImage = NULL;
    _tabIndex = -1;
    _resultIndex = 0;

    _zoomFitMode = false;

    _scale = 1.0;
    _offsetX = 0;
    _offsetY = 0;

    // add scene
    _pixmapItem = new IPPixmapItem(this);
    _graphicsScene = new QGraphicsScene(this);
    _graphicsScene->addItem(_pixmapItem);

    // add view
    _graphicsView = new IPImageViewerGraphicsView(this);
    _graphicsView->setScene(_graphicsScene);
    _graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    _graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    _graphicsView->setAlignment(Qt::AlignCenter);

    // prevent scrollbars from catching mouse wheel events
    horizontalScrollBar()->installEventFilter(this);
    verticalScrollBar()->installEventFilter(this);


    layout()->addWidget(_graphicsView);

    setMouseTracking(true);
    _graphicsView->setMouseTracking(true);

    connect(_graphicsView->horizontalScrollBar(), &QAbstractSlider::valueChanged, _imageViewerWindow, &ImageViewerWindow::on_horizontalScrollBarChanged);
    connect(_graphicsView->verticalScrollBar(), &QAbstractSlider::valueChanged, _imageViewerWindow, &ImageViewerWindow::on_verticalScrollBarChanged);
}

IPImageViewer::~IPImageViewer()
{
    delete _image;
}

void IPImageViewer::updateImage()
{
    if(_processStep && _processStep->process() && _processStep->process()->isResultReady())
    {
        // delete last image
        delete _image;
        _image = NULL;

        _rawData = NULL;
        _rawImage = NULL;

        // convert from IPLImage
        _rawData = _processStep->process()->getResultData(_resultIndex);

        // if the result is invalid, abort
        if(!_rawData)
        {
            setVisible(false);
            return;
        }
        else
        {
            setVisible(true);
        }

        if(_rawData->type() == IPLData::IMAGE_COLOR
                || _rawData->type() == IPLData::IMAGE_GRAYSCALE
                || _rawData->type() == IPLData::IMAGE_BW
                || _rawData->type() == IPLData::IMAGE_ORIENTED
                )
        {
            _rawImage = _rawData->toImage();

            // show normal image
            _image = new QImage(_rawImage->rgb32(), _rawImage->width(), _rawImage->height(), QImage::Format_RGB32);
        }
        else if(_rawData->type() == IPLData::IMAGE_COMPLEX)
        {
            _rawComplexImage = _rawData->toComplexImage();

            // show complex image
            _image = new QImage(_rawComplexImage->rgb32(), _rawComplexImage->width(), _rawComplexImage->height(), QImage::Format_RGB32);
        }
        else if(_rawData->type() == IPLImage::POINT)
        {
            // show point
            _rawData = _processStep->process()->getResultData(0);
            _image = new QImage(_rawData->toImage()->rgb32(), _rawData->toImage()->width(), _rawData->toImage()->height(), QImage::Format_RGB32);

            QPainter painter(_image);
            painter.setRenderHint(QPainter::Antialiasing, true);

            IPLPoint* p = _processStep->process()->getResultData(1)->toPoint();

            QPoint point;
            point.setX(p->x());
            point.setY(p->y());

            qDebug() << point;

            QBrush brush(Qt::red);
            painter.setBrush(brush);
            painter.drawEllipse(point, 10, 10);
        }
        else if(_rawData->type() == IPLData::MATRIX)
        {
            int cellSize = 30;
            int headerSize = 30;

            IPLMatrix* matrix = _rawData->toMatrix();
            _image = new QImage(matrix->width()*cellSize+2*headerSize, matrix->height()*cellSize+2*headerSize, QImage::Format_RGB32);
            _image->fill(Qt::white);

            QPainter painter(_image);
            painter.setRenderHint(QPainter::Antialiasing, true);

            // header
            QPen pen(Qt::black);
            pen.setWidth(2);
            painter.setPen(pen);

            QString title("%1×%2 Matrix");
            painter.drawText(headerSize+2, headerSize-5, title.arg(matrix->width()).arg(matrix->height()));

            // content
            for(int y=0; y < matrix->height(); y++)
            {
                for(int x=0; x < matrix->width(); x++)
                {
                    QRectF box(x*cellSize+headerSize, y*cellSize+headerSize, cellSize, cellSize);
                    painter.drawText(box, Qt::AlignCenter|Qt::AlignVCenter, QString::number(matrix->get(x,y), 'f', 2));
                    painter.drawRect(box);
                }
            }
        }

        // update histogram
        _imageViewerWindow->updateHistogram(_rawImage);

        // update statistics
        _imageViewerWindow->updateStatistics(_rawImage);

        // update zoom widget
        _imageViewerWindow->updateZoomwidget(_rawImage);


        if(_image)
        {
            // we make a copy of the image data in order to prevent read access violations
            QPixmap pixmap = QPixmap::fromImage(_image->copy());

            _pixmapItem->setPixmap(pixmap);

            // center to 0,0
//            _pixmapItem->setPos(-_pixmapItem->boundingRect().width()/2, -_pixmapItem->boundingRect().height()/2);

            _graphicsScene->setSceneRect(_pixmapItem->boundingRect());
        }
        else
        {
            _pixmapItem->hide();
        }
        //_graphicsScene->update();

        // update title
        /*if(_tabIndex > -1)
        {
            IPLProcessPropertyString* property = (IPLProcessPropertyString*) _processStep->process()->property("title");
            QString title = QString::fromStdString(property->value());
            ((QTabWidget*) parent())->setTabText(_tabIndex, title);
        }*/
    }
}

void IPImageViewer::updateMousePosition(int x, int y)
{
    emit mousePositionChanged(x, y);
}

void IPImageViewer::on_mouseClicked()
{
    emit mouseClicked();
}


void IPImageViewer::on_mouseDoubleClicked()
{
    emit mouseDoubleClicked();
}



void IPImageViewer::zoomIn()
{
    _zoomFitMode = false;

    if(_scale >= 9.0)
        return;

    _scale *= 1.1;

    QMatrix matrix;
    matrix.scale(_scale, _scale);
    _graphicsView->setMatrix(matrix);
}

void IPImageViewer::zoomOut()
{
    _zoomFitMode = false;

    if(_scale <= 0.1)
        return;

    _scale *= (1/1.1);

    QMatrix matrix;
    matrix.scale(_scale, _scale);
    _graphicsView->setMatrix(matrix);
}

void IPImageViewer::zoomFit()
{
    _zoomFitMode = true;

    _graphicsView->fitInView(_pixmapItem->boundingRect(), Qt::KeepAspectRatio);

    _scale = _graphicsView->matrix().m11();
}

void IPImageViewer::zoomReset()
{
    _zoomFitMode = false;

    _scale = 1.0;

    QMatrix matrix;
    matrix.scale(_scale, _scale);
    _graphicsView->setMatrix(matrix);
}

bool IPImageViewer::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::Wheel)
    {
        return true; // filter the original event out
    }
    return false; // pass other events
}

void IPImageViewer::showEvent(QShowEvent *)
{
    //updateImage();

    if(_zoomFitMode)
    {
        zoomFit();
        emit zoomChanged(zoomFactor());
    }
}

void IPImageViewer::resizeEvent(QResizeEvent *)
{
    if(_zoomFitMode)
    {
        zoomFit();
        emit zoomChanged(zoomFactor());
    }
}

void IPImageViewerGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    ((IPImageViewer*) parent())->on_mouseClicked();

    QGraphicsView::mouseReleaseEvent(event);
}

void IPImageViewerGraphicsView::mouseDoubleClickEvent(QMouseEvent* event)
{
    ((IPImageViewer*) parent())->on_mouseDoubleClicked();
}

void IPPixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    int posX = (int) event->pos().x() + 0.5; // round to next full pixel
    int posY = (int) event->pos().y() + 0.5; // round to next full pixel

    _imageViewer->updateMousePosition(posX, posY);
}
