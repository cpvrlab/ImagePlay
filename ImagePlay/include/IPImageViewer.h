//#############################################################################
//  File:      IPImageViewer.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPIMAGEVIEWER_H
#define IPIMAGEVIEWER_H

#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>
#include <QPixmap>
#include <QNativeGestureEvent>

#include <QDebug>

#include "IPLProcess.h"
#include "IPLImage.h"
#include "ImageViewerWindow.h"

#include "IPProcessStep.h"

class IPProcessStep;
class ImageViewerWindow;
class IPImageViewer;


//-----------------------------------------------------------------------------
//!Custom QGraphicsView for handling mouse events
class IPImageViewerGraphicsView : public QGraphicsView
{
public:
    IPImageViewerGraphicsView(QWidget * parent = 0) : QGraphicsView(parent)
    {
        setMouseTracking(true);
    }

    // catch OS X pinch to zoom events
    bool event(QEvent *event) {
        if (event->type() == QEvent::NativeGesture) {
            QNativeGestureEvent *nge = static_cast<QNativeGestureEvent *>(event);

            if (nge->gestureType() == Qt::ZoomNativeGesture) {
                double v = nge->value();

                if (v < 0)
                    qDebug() << "Pinch to zoom out"; //((IPImageViewer*)parent())->zoomOut();
                else
                    qDebug() << "Pinch to zoom in"; //((IPImageViewer*)parent())->zoomIn();

                // It seems the NativeGestureEvent::pos() incorrectly reports global coordinates
                //QPoint g = mapFromGlobal(nge->globalPos());
            }

            return true;
        }
        return QWidget::event(event);
    }

//    void wheelEvent(QWheelEvent* event)     { event->ignore(); }
    void mouseReleaseEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent *);
};

//-----------------------------------------------------------------------------
//!Custom QGraphicsPixmapItem for handling mouse events
class IPPixmapItem : public QGraphicsPixmapItem
{
public:
    IPPixmapItem(IPImageViewer* imageViewer, QGraphicsItem * parent = 0) : QGraphicsPixmapItem(parent)
    {
        _imageViewer = imageViewer;

        setAcceptHoverEvents(true);
        setCursor(Qt::CrossCursor);
    }

    // QGraphicsItem interface
protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
private:
    IPImageViewer* _imageViewer;
};

//-----------------------------------------------------------------------------
//!IPImageViewer displays the result of one IPProcessStep
/*! Handles IPLData to QImage conversion and all interactions like zooming
 * and paning.
 */
class IPImageViewer : public QFrame
{
    Q_OBJECT
public:
    explicit IPImageViewer(ImageViewerWindow*, QWidget *parent = 0);
    virtual ~IPImageViewer();
    void attachProcessStep(IPProcessStep* step, int resultIndex) { _processStep = step; _resultIndex = resultIndex; }
    void updateImage();
    IPProcessStep* processStep()                { return _processStep; }
    void setTabIndex(int tabIndex)              { _tabIndex = tabIndex; }
    void setResultIndex(int index)              { _resultIndex = index; }
    int resultIndex()                           { return _resultIndex; }
    int zoomFactor()                            { return _scale*100; }
    IPLData* rawData()                          { return _rawData; }
    QImage*  image()                            { return _image; }
    void updateMousePosition(int, int);
signals:
    void zoomChanged(int);
    void mousePositionChanged(int, int);
    void mouseClicked();
    void mouseDoubleClicked();

public slots:
    void on_mouseClicked();
    void on_mouseDoubleClicked();

private:
    QImage*             _image;
    IPLImage*           _rawImage;
    IPLComplexImage*    _rawComplexImage;
    IPLData*            _rawData;
    QPixmap             _pixmap;
    IPPixmapItem*       _pixmapItem;
    IPProcessStep*      _processStep;
    IPImageViewerGraphicsView*      _graphicsView;
    QGraphicsScene*     _graphicsScene;
    int                 _resultIndex;
    int                 _tabIndex;
    ImageViewerWindow*  _imageViewerWindow;
    double              _scale;
    int                 _offsetX;
    int                 _offsetY;
    bool                _zoomFitMode;
    int                 _horizontalScrollValue;
    int                 _verticalScrollValue;

public:
    void zoomIn();
    void zoomOut();
    void zoomFit();
    void zoomReset();

    bool eventFilter(QObject* object, QEvent* event);

    QScrollBar* horizontalScrollBar()   { return _graphicsView->horizontalScrollBar(); }
    QScrollBar* verticalScrollBar()     { return _graphicsView->verticalScrollBar(); }

    // QWidget interface
protected:
    void showEvent(QShowEvent *);
    void resizeEvent(QResizeEvent *);
};

#endif // IPIMAGEVIEWER_H
