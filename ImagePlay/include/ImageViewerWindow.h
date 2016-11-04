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

#ifndef IMAGEVIEWERWINDOW_H
#define IMAGEVIEWERWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QToolButton>
#include <QTabBar>
#include <QColor>

#include "IPL_processes.h"
#include "IPImageViewer.h"
#include "IPProcessStep.h"
#include "IPHistogramWidget.h"
#include "MainWindow.h"
#include "PropertyWidgets/IPPropertyWidget.h"

namespace Ui {
class ImageViewerWindow;
}

class IPProcessStep;
class IPImageViewer;
class MainWindow;
class IPZoomWidget;

//-----------------------------------------------------------------------------
//!ImageViewerWindow provides a window for viewing all process results
/*!Every IPLProcess is represented by a tab in the ImageViewerWindow. Additional
 * information such as a histogram, statistics and a zoomwidget are displayed
 * on the right side.
*/
class ImageViewerWindow : public QMainWindow, public IPLCoordinatePickProvider, public IPLColorPickProvider
{
    Q_OBJECT

public:
    enum ZoomAction
    {
        ZOOM_NONE,  //!<
        ZOOM_IN,    //!< increase zoom by 10%
        ZOOM_OUT,   //!< decrease zoom by 10%
        ZOOM_FIT,   //!< zoom to fit the image inside the current window
        ZOOM_RESET  //!< zoom to 100%
    };

    explicit ImageViewerWindow(MainWindow *parent = 0);
    ~ImageViewerWindow();

    void addProcessStep(IPProcessStep* processStep);
    void removeProcessStep(IPProcessStep* processStep);
    void attachProcess(IPLProcess* process);
    void updateImage();

    void setActiveStep(long stepID);
    void showProcessDuration(int durationMs);

    void updateHistogram(IPLImage*);
    void resetHistogramValue();

    void updateStatistics(IPLImage*);
    void resetStatistics();

    void updateZoomwidget(IPLImage*);
    void resetZoomWidget();

    // disable right click toolbar menu
    QMenu* createPopupMenu()    { return NULL; }

    void sortTabs();

    QSize lastSize()            { return  _lastSize; }
    QPoint lastPos()            { return  _lastPos; }

    void zoomAllViewers(ZoomAction action);
    void scrollAllViewers(int horizontal, int vertical);

    int zoomWidgetMode();
    void setZoomWidgetMode(int mode);

    void setColorPickHandler(IPLColorPickHandler* widget);
    void setCoordinatePickHandler(IPLCoordinatePickHandler* widget);

    void clearColorPickHandler();
    void clearCoordinatePickHandler();

    void setIgnoreMouseEvents(bool state)           { _ignoreMouseEvents = state; }

    void updateOutputs();
    IPZoomWidget *zoomWidget();

public slots:
    void on_actionNextImage_triggered();
    void on_actionPrevImage_triggered();
    void on_actionShowGrid_triggered(bool checked);
    void tabChanged(int);
    void on_radioLinear_clicked();
    void on_radioLogarithmic_clicked();
    void histogramHighlightChangedGrayscale(int position, int value, float percentage);
    void histogramHighlightChangedColor(int position, int r, int g, int b, float percentageR, float percentageG, float percentageB);
    void on_actionHideSidebar_triggered(bool checked);
    void on_btnZoomIn_clicked();
    void on_btnZoomOut_clicked();
    void on_btnZoomFit_clicked();
    void on_btnZoomReset_clicked();
    void on_horizontalScrollBarChanged(int);
    void on_verticalScrollBarChanged(int);
    void on_zoomChanged(int);
    void on_comboBoxResults_activated(int index);
    void on_mousePositionChanged(int, int);
    void on_mouseClick();
    void on_mouseDoubleClick();
    void on_zoomWidgetModeCombobox_currentIndexChanged(int index);

private:
    Ui::ImageViewerWindow *ui;
    QMap<int, IPImageViewer*>   _imageViewers1;
    QMap<int, IPImageViewer*>   _imageViewers2;
    int                         _gridLayoutCounter;
    QGridLayout*                _gridLayout;
    MainWindow*                 _mainWindow;
    QButtonGroup                _histogramRadioGroup;
    QSize                       _lastSize;
    QPoint                      _lastPos;
    int                         _horizontalScrollValue;
    int                         _verticalScrollValue;
    IPLColorPickHandler*        _colorPickHandler;
    IPLCoordinatePickHandler*   _coordinatePickHandler;
    QColor                      _currentColor;
    QPoint                      _currentPosition;
    bool                        _ignoreZoomEvents;
    bool                        _ignoreScrollEvents;
    bool                        _ignoreMouseEvents;
    ZoomAction                  _currentZoomMode;
    double                      _currentZoomFactor;


    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *);
    void moveEvent(QMoveEvent *);
    void wheelEvent(QWheelEvent *);
    void closeEvent(QCloseEvent *);
};

#endif // IMAGEVIEWERWINDOW_H




