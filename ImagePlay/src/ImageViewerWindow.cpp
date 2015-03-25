#include "ImageViewerWindow.h"
#include "ui_ImageViewerWindow.h"

//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::ImageViewerWindow
*/
ImageViewerWindow::ImageViewerWindow(MainWindow *mainWindow) :
    QMainWindow(mainWindow),
    ui(new Ui::ImageViewerWindow)
{
    ui->setupUi(this);

    setWindowTitle("Image Viewer");
    setWindowIcon(QIcon(":/icons/30.png"));

    _mainWindow = mainWindow;

    _colorPickHandler = NULL;
    _coordinatePickHandler = NULL;

//    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    setPalette(darkPalette);

    _gridLayoutCounter = 0;
    _gridLayout = new QGridLayout;
    ui->gridWidget->setLayout(_gridLayout);

    ui->tabWidget->setVisible(true);
    ui->gridWidget->setVisible(false);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &ImageViewerWindow::tabChanged);

    // radio buttons
    ui->radioLinear->setChecked(true);
    _histogramRadioGroup.addButton(ui->radioLinear, 0);
    _histogramRadioGroup.addButton(ui->radioLogarithmic, 1);

    // histogram
    connect(ui->histogramWidget, &IPHistogramWidget::highlightChangedGrayscale, this, &ImageViewerWindow::histogramHighlightChangedGrayscale);
    connect(ui->histogramWidget, &IPHistogramWidget::highlightChangedColor, this, &ImageViewerWindow::histogramHighlightChangedColor);
    connect(ui->histogramWidget, &IPHistogramWidget::resetHighlightValue, this, &ImageViewerWindow::resetHistogramValue);

    // remove titlebar
    ui->dockWidget->setTitleBarWidget(new QWidget(this));

    QWidget* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->toolBar->insertWidget(ui->actionHideSidebar, spacer);

    _horizontalScrollValue = 0;
    _verticalScrollValue = 0;

}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::~ImageViewerWindow
*/
ImageViewerWindow::~ImageViewerWindow()
{
    delete ui;
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::addProcessStep
*/
void ImageViewerWindow::addProcessStep(IPProcessStep *processStep)
{
    int stepID = processStep->stepID();
    if(_imageViewers1.contains(stepID))
    {
        return;
    }

    // create new tab
    if(processStep->process())
    {
        IPImageViewer* imageViewer1 = new IPImageViewer(this, ui->tabWidget);
        connect(imageViewer1, &IPImageViewer::scrollBarsChanged, this, &ImageViewerWindow::on_scrollBarsChanged);
        connect(imageViewer1, &IPImageViewer::zoomChanged, this, &ImageViewerWindow::on_zoomChanged);
        connect(imageViewer1, &IPImageViewer::mousePositionChanged, this, &ImageViewerWindow::on_mousePositionChanged);
        connect(imageViewer1, &IPImageViewer::mouseClicked, this, &ImageViewerWindow::on_mouseClick);
        connect(imageViewer1, &IPImageViewer::mouseDoubleClicked, this, &ImageViewerWindow::on_mouseDoubleClick);

//        IPImageViewer* imageViewer2 = new IPImageViewer(this, this);
        imageViewer1->attachProcessStep(processStep, 0);
//        imageViewer2->attachProcessStep(processStep, 0);

        // some styling
//        imageViewer2->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

        QString tabName = QString::number(stepID).append(": ").append(QString::fromStdString(processStep->process()->title()));

        // add tab
        ui->tabWidget->addTab(imageViewer1, tabName);

        // add tab data for sorting
        //

        // add grid item
//        _gridLayout->addWidget(imageViewer2, _gridLayoutCounter/2, _gridLayoutCounter%2);
//        _gridLayoutCounter++;

        _imageViewers1.insert(stepID, imageViewer1);
//        _imageViewers2.insert(stepID, imageViewer2);
    }
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::removeProcessStep
*/
void ImageViewerWindow::removeProcessStep(IPProcessStep *processStep)
{
    long stepID = processStep->stepID();
    delete _imageViewers1.value(stepID);
    _imageViewers1.remove(stepID);

    // reset zoom histogram, statistics and zoom
    updateHistogram(NULL);
    updateStatistics(NULL);
    updateZoomwidget(NULL);

    resetHistogramValue();
    resetStatistics();
    resetZoomWidget();

//    delete _imageViewers2.value(stepID);
//    _imageViewers2.remove(stepID);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::updateImage
*/
void ImageViewerWindow::updateImage()
{
    if(ui && ui->tabWidget->isVisible())
    {
        // update the currently visible image
        IPImageViewer* imageViewer = ((IPImageViewer*) ui->tabWidget->currentWidget());

        if(imageViewer)
           imageViewer->updateImage();
    }
    else
    {
        // update all images on grid
        /*for(auto it = _imageViewers2.begin(); it != _imageViewers2.end(); ++it)
        {
            IPImageViewer* item = it.value();
            item->updateImage();
        }*/
    }
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::setActiveStep
*/
void ImageViewerWindow::setActiveStep(long stepID)
{
    for(int i=0; i < ui->tabWidget->count(); i++)
    {
        IPImageViewer* item = (IPImageViewer*) ui->tabWidget->widget(i);
        if(item->processStep()->stepID() == stepID)
        {
            ui->tabWidget->setCurrentIndex(i);
            return;
        }
    }
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::showProcessDuration
*/
void ImageViewerWindow::showProcessDuration(int durationMs)
{
    if(durationMs > 0)
    {
        float fps = 1000.0f / durationMs;
        QString durationMsg("Total duration: %1ms / %2fps");
        ui->statusbar->showMessage(durationMsg.arg(durationMs).arg(fps, 0, 'f', 1));
    }
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::updateHistogram
*/
void ImageViewerWindow::updateHistogram(IPLImage* image)
{
    ui->histogramWidget->updateHistogram(image);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::resetHistogramValue
*/
void ImageViewerWindow::resetHistogramValue()
{
    ui->labelHistogramValue->setText("-");
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::updateStatistics
*/
void ImageViewerWindow::updateStatistics(IPLImage* image)
{
    if(!image)
        return;

    // only update if panel is not hidden
    if(ui->statisticsLabel->isVisible())
    {
        // image information
        QString statistics("<table>");
        statistics.append("<tr><td><b>Height: </b></td><td>%1px</td></tr>");
        statistics.append("<tr><td><b>Width: </b></td><td>%2px</td></tr>");
        statistics.append("<tr><td><b>Image Type: </b></td><td>%3</td></tr>");
        statistics.append("<tr><td><b>Image Planes: </b></td><td>%4</td></tr>");
        statistics.append("</table>");

        QString imageType = "COLOR";
        if(image->type() == IPLData::IMAGE_GRAYSCALE)
            imageType = "GRAY";
        else if(image->type() == IPLImage::IMAGE_BW)
            imageType = "BW";

        statistics = statistics.arg(image->height()).arg(image->width()).arg(imageType).arg(image->getNumberOfPlanes());


        // histogram statistics

        if(image->type() == IPLData::IMAGE_COLOR)
        {
            IPLHistogram* histogramR = ui->histogramWidget->histogramR();
            IPLHistogram* histogramG = ui->histogramWidget->histogramG();
            IPLHistogram* histogramB = ui->histogramWidget->histogramB();

            QString statistics2("<table>");
            statistics2.append("<tr><td><b>Min: </b></td><td style=\"color:#FF0000;\">%1 </td><td style=\"color:#41DB00;\">%2 </td><td style=\"color:#0094FF;\">%3</td></tr>");
            statistics2 = statistics2.arg(histogramR->minLevel()).arg(histogramG->minLevel()).arg(histogramB->minLevel());
            statistics2.append("<tr><td><b>Max: </b></td><td style=\"color:#FF0000;\">%1 </td><td style=\"color:#41DB00;\">%2 </td><td style=\"color:#0094FF;\">%3</td></tr>");
            statistics2 = statistics2.arg(histogramR->maxLevel()).arg(histogramG->maxLevel()).arg(histogramB->maxLevel());
            statistics2.append("<tr><td><b>Mean: </b></td><td style=\"color:#FF0000;\">%1 </td><td style=\"color:#41DB00;\">%2 </td><td style=\"color:#0094FF;\">%3</td></tr>");
            statistics2 = statistics2.arg(histogramR->meanLevel()).arg(histogramG->meanLevel()).arg(histogramB->meanLevel());
            statistics2.append("<tr><td><b>Median: </b></td><td style=\"color:#FF0000;\">%1 </td><td style=\"color:#41DB00;\">%2 </td><td style=\"color:#0094FF;\">%3</td></tr>");
            statistics2 = statistics2.arg(histogramR->medianLevel()).arg(histogramG->medianLevel()).arg(histogramB->medianLevel());
            statistics2.append("<tr><td><b>Mode: </b></td><td style=\"color:#FF0000;\">%1 </td><td style=\"color:#41DB00;\">%2 </td><td style=\"color:#0094FF;\">%3</td></tr>");
            statistics2 = statistics2.arg(histogramR->modeLevel()).arg(histogramG->modeLevel()).arg(histogramB->modeLevel());

            statistics2.append("</table>");

            statistics += statistics2;
        }
        else if(image->type() == IPLData::IMAGE_GRAYSCALE || image->type() == IPLImage::IMAGE_BW)
        {
            IPLHistogram* histogram = ui->histogramWidget->histogram();

            QString statistics2("<table>");
            statistics2.append("<tr><td><b>Min: </b></td><td>%1</td></tr>");
            statistics2 = statistics2.arg(histogram->minLevel());
            statistics2.append("<tr><td><b>Max: </b></td><td>%1</td></tr>");
            statistics2 = statistics2.arg(histogram->maxLevel());
            statistics2.append("<tr><td><b>Mean: </b></td><td>%1</td></tr>");
            statistics2 = statistics2.arg(histogram->meanLevel());
            statistics2.append("<tr><td><b>Median: </b></td><td>%1</td></tr>");
            statistics2 = statistics2.arg(histogram->medianLevel());
            statistics2.append("<tr><td><b>Mode: </b></td><td>%1</td></tr>");
            statistics2 = statistics2.arg(histogram->modeLevel());
            statistics2.append("</table>");

            statistics += statistics2;
        }

        // display text
        ui->statisticsLabel->setText(statistics);
    }
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::resetStatistics
*/
void ImageViewerWindow::resetStatistics()
{
    ui->statisticsLabel->setText("-");
}

void ImageViewerWindow::updateZoomwidget(IPLImage* image)
{
    ui->zoomWidget->setImage(image);
}

void ImageViewerWindow::resetZoomWidget()
{
    ui->zoomWidget->setImage(NULL);
}

//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::sortTabs
*/
void ImageViewerWindow::sortTabs()
{
    // sort tabs according to their process order
    /*for(int i=0; i<ui->tabWidget->tabBar()->count(); i++)
    {
        ui->tabWidget->tabBar()->setTabData(i, 123);
        QVariant data = ui->tabWidget->tabBar()->tabData(i);
        qDebug() << "TAB: " << data;
    }*/
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::zoomAllViewers
*/
void ImageViewerWindow::zoomAllViewers(ZoomAction action)
{
    // apply zoom and scroll position to all viewers
//    int horizontalScroll = ((IPImageViewer*) ui->tabWidget->currentWidget())->horizontalScrollBar()->value();
//    int verticalScroll = ((IPImageViewer*) ui->tabWidget->currentWidget())->verticalScrollBar()->value();

    QMapIterator<int, IPImageViewer*> it(_imageViewers1);
    int zoomFactor = 0;
    while (it.hasNext())
    {
        try
        {
            it.next();

            if(action == ImageViewerWindow::ZOOM_IN)
                it.value()->zoomIn();
            else if(action == ImageViewerWindow::ZOOM_OUT)
                it.value()->zoomOut();
            else if(action == ImageViewerWindow::ZOOM_FIT)
                it.value()->zoomFit();
            else if(action == ImageViewerWindow::ZOOM_RESET)
                it.value()->zoomReset();

            zoomFactor = it.value()->zoomFactor();

            it.value()->horizontalScrollBar()->setValue(_horizontalScrollValue);
            it.value()->verticalScrollBar()->setValue(_verticalScrollValue);
        }
        catch(std::exception& e)
        {
            qWarning() << "Error when zooming: " << e.what();
        }
    }

    on_zoomChanged(zoomFactor);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::zoomWidgetMode
*/
int ImageViewerWindow::zoomWidgetMode()
{
    return ui->zoomWidgetModeCombobox->currentIndex();
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::setZoomWidgetMode
*/
void ImageViewerWindow::setZoomWidgetMode(int mode)
{
    ui->zoomWidgetModeCombobox->setCurrentIndex(mode);
}

void ImageViewerWindow::setColorPickHandler(IPLColorPickHandler *widget)
{
    _colorPickHandler = widget;
}

void ImageViewerWindow::setCoordinatePickHandler(IPLCoordinatePickHandler *widget)
{
    _coordinatePickHandler = widget;
}

void ImageViewerWindow::clearColorPickHandler()
{
    _colorPickHandler = NULL;
}

void ImageViewerWindow::clearCoordinatePickHandler()
{
    _coordinatePickHandler = NULL;
}


//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_actionNextImage_triggered
*/
void ImageViewerWindow::on_actionNextImage_triggered()
{
    int newIndex = ui->tabWidget->currentIndex() + 1;
    newIndex = newIndex > (ui->tabWidget->count()-1) ? 0 : newIndex;
    ui->tabWidget->setCurrentIndex(newIndex);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_actionPrevImage_triggered
*/
void ImageViewerWindow::on_actionPrevImage_triggered()
{
    int newIndex = ui->tabWidget->currentIndex() - 1;
    newIndex = newIndex < 0 ? (ui->tabWidget->count()-1) : newIndex;
    ui->tabWidget->setCurrentIndex(newIndex);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_actionShowGrid_triggered
*/
void ImageViewerWindow::on_actionShowGrid_triggered(bool checked)
{
    if(checked)
    {
        ui->dockWidget->setVisible(false);
        ui->tabWidget->setVisible(false);
        ui->gridWidget->setVisible(true);
        ui->actionHideSidebar->setVisible(false);
    }
    else
    {
        ui->dockWidget->setVisible(true);
        ui->tabWidget->setVisible(true);
        ui->gridWidget->setVisible(false);
        ui->actionHideSidebar->setVisible(true);
    }
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::tabChanged
*/
void ImageViewerWindow::tabChanged(int tabIndex)
{
    IPImageViewer* item = (IPImageViewer*) ui->tabWidget->widget(tabIndex);

    // hightlight grid item
    if(item && item->processStep() && _mainWindow->synchronizeViews())
    {
        _mainWindow->setActiveProcessStep(item->processStep());
    }

    // update result selector
    if(item && item->processStep())
    {
        ui->comboBoxResults->clear();

        // add result names
        for(size_t i=0; i<item->processStep()->process()->outputs()->size(); i++)
        {
            IPLProcessIO* processIO = &item->processStep()->process()->outputs()->at(i);
            QString name = QString::fromStdString(processIO->name);
            ui->comboBoxResults->addItem(name);
        }

        // select current result
        int index = item->resultIndex();
        ui->comboBoxResults->setCurrentIndex(index);
    }

    updateImage();
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_radioLinear_clicked
Changes the histogram display mode to linear
*/
void ImageViewerWindow::on_radioLinear_clicked()
{
    ui->histogramWidget->setLogarithmic(false);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_radioLogarithmic_clicked
Changes the histogram display mode to logarighmic
*/
void ImageViewerWindow::on_radioLogarithmic_clicked()
{
    ui->histogramWidget->setLogarithmic(true);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::histogramHighlightChangedGrayscale
*/
void ImageViewerWindow::histogramHighlightChangedGrayscale(int position, int value, float percentage)
{
    QString text("<table width=\"200\"><tr>");
    text.append("<td width=\"70\">Level: %1</td> ");
    text.append("<td width=\"130\">Value: %2 (%3%)</td>");
    text.append("</tr></table>");

    text = text.arg(position).arg(value).arg(percentage, 0, 'f', 1);

    ui->labelHistogramValue->setText(text);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::histogramHighlightChangedColor
*/
void ImageViewerWindow::histogramHighlightChangedColor(int position, int r, int g, int b, float percentageR, float percentageG, float percentageB)
{
    QString text("<table width=\"200\"><tr>");
    text.append("<td colspan=\"3\">Level: %1</td>");
    text.append("</tr><tr>");
    text.append("<td width=\"66\" style=\"color: #FF0000\">R: %2</td>");
    text.append("<td width=\"66\" style=\"color: #41DB00\">G: %3</td>");
    text.append("<td width=\"66\" style=\"color: #0094FF\">B: %4</td>");
    text.append("</tr><tr>");
    text.append("<td width=\"66\" style=\"color: #FF0000\">(%5%)</td>");
    text.append("<td width=\"66\" style=\"color: #41DB00\">(%6%)</td>");
    text.append("<td width=\"66\" style=\"color: #0094FF\">(%7%)</td>");
    text.append("</tr></table>");

    text = text.arg(position).arg(r).arg(g).arg(b).arg(percentageR, 0, 'f', 1).arg(percentageG, 0, 'f', 1).arg(percentageB, 0, 'f', 1);

    ui->labelHistogramValue->setText(text);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_actionHideSidebar_triggered
*/
void ImageViewerWindow::on_actionHideSidebar_triggered(bool hide)
{
    if(hide)
    {
        ui->dockWidget->setVisible(false);
        ui->actionHideSidebar->setText("<");
    }
    else
    {
        ui->dockWidget->setVisible(true);
        ui->actionHideSidebar->setText(">");
    }
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::resizeEvent
*/
void ImageViewerWindow::resizeEvent(QResizeEvent *)
{
    _lastSize = size();
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::moveEvent
*/
void ImageViewerWindow::moveEvent(QMoveEvent *)
{
    _lastPos = pos();
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::wheelEvent
Custom mouse wheel handler for zooming the image
*/
void ImageViewerWindow::wheelEvent(QWheelEvent* event)
{
    if(event->angleDelta().y() < 0)
        zoomAllViewers(ZOOM_OUT);
    else
        zoomAllViewers(ZOOM_IN);

    event->ignore();
}

void ImageViewerWindow::closeEvent(QCloseEvent* e)
{
    // tell the MainWindow that the image viewer is no longer visible
    _mainWindow->on_actionImageViewer_hidden();
    QMainWindow::closeEvent(e);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_btnZoomIn_clicked
*/
void ImageViewerWindow::on_btnZoomIn_clicked()
{
    zoomAllViewers(ZOOM_IN);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_btnZoomOut_clicked
*/
void ImageViewerWindow::on_btnZoomOut_clicked()
{
    zoomAllViewers(ZOOM_OUT);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_btnZoomFit_clicked
*/
void ImageViewerWindow::on_btnZoomFit_clicked()
{
    zoomAllViewers(ZOOM_FIT);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_btnZoomReset_clicked
*/
void ImageViewerWindow::on_btnZoomReset_clicked()
{
    zoomAllViewers(ZOOM_RESET);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_scrollBarsChanged

@TODO scrollbars currently don't work
*/
void ImageViewerWindow::on_scrollBarsChanged(int horizontal, int vertical)
{
    _horizontalScrollValue = horizontal;
    _verticalScrollValue = vertical;

    zoomAllViewers(ImageViewerWindow::ZOOM_NONE);
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_zoomChanged
Updates the zoom information in percent.
*/
void ImageViewerWindow::on_zoomChanged(int zoomFactor)
{
    if(zoomFactor)
    {
        QString zoom("%1%"); // x%
        ui->zoomLabel->setText(zoom.arg(zoomFactor));
    }
    else
    {
        ui->zoomLabel->setText("");
    }
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_comboBoxResults_activated
Switches between the different result outputs of a process step.
For example R, G and B channels on a channel splitter.
*/
void ImageViewerWindow::on_comboBoxResults_activated(int index)
{
    // set result index
    ((IPImageViewer*) ui->tabWidget->currentWidget())->setResultIndex(index);
    ((IPImageViewer*) ui->tabWidget->currentWidget())->updateImage();
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_mousePositionChanged
Updates the position of the zoomWindow and the pixel information window
*/
void ImageViewerWindow::on_mousePositionChanged(int x, int y)
{
    // check if we have a valid image
    if(!((IPImageViewer*) ui->tabWidget->currentWidget())->image())
        return;

    if(x < 0 || y < 0)
    {
        return;
    }

    if(ui->zoomWidget->isPositionLocked())
        return;

    // save current color and position for later usage by pickhandlers
    _currentColor = QColor(((IPImageViewer*) ui->tabWidget->currentWidget())->image()->pixel(x, y));
    _currentPosition = QPoint(x,y);

    IPLData* data = NULL;
    if(((IPImageViewer*) ui->tabWidget->currentWidget())->rawData())
    {
        data = ((IPImageViewer*) ui->tabWidget->currentWidget())->rawData();
        if(!data)
            return;
    }

    QString message("<table><tr>");
    message.append("<td><b>Position: </b></td>");
    message.append("<td>x: %1, y: %2</td>");
    message.append("</tr><tr>");
    message.append("<td><b>Center Value: </b></td>");
    message.append("<td>%3</td>");
    message.append("</tr></table>");


    QString value;
    if(data->type() == IPLData::IMAGE_COLOR)
    {
        IPLImage* image = data->toImage();
        QString r = QString::number(image->plane(0)->cp(x, y), 'f', 2);
        QString g = QString::number(image->plane(1)->cp(x, y), 'f', 2);
        QString b = QString::number(image->plane(2)->cp(x, y), 'f', 2);
        value.append("<table width=\"120\"><tr>");
        value.append("<td align=\"right\" style=\"color:#FF0000;\">%1</td>");
        value.append("<td align=\"right\" style=\"color:#41DB00;\">%2</td>");
        value.append("<td align=\"right\" style=\"color:#0094FF;\">%3</td>");
        value.append("</tr></table>");

        value = value.arg(r).arg(g).arg(b);
    }
    else if(data->type() == IPLData::IMAGE_GRAYSCALE || data->type() == IPLImage::IMAGE_BW)
    {
        IPLImage* image = data->toImage();
        QString v = QString::number(image->plane(0)->cp(x, y), 'f', 2);
        value.append("<span>%1</span>");

        value = value.arg(v);
    }
    else if(data->type() == IPLData::IMAGE_ORIENTED)
    {
        IPLImage* image = data->toImage();
        QString m = QString::number(image->plane(0)->cp(x, y), 'f', 2);
        QString p = QString::number(image->plane(1)->cp(x, y), 'f', 2);
        value.append("<span>Phase: %1, Magnitude: %2</span>");

        value = value.arg(p).arg(m);
    }
    else if(data->type() == IPLData::IMAGE_COMPLEX)
    {
        IPLComplexImage* image = data->toComplexImage();
        int xx = ((x-image->width()/2) + image->width()) % image->width();
        int yy = ((y-image->height()/2) + image->height()) % image->height();

        // make 0/0 the center
        x = x-image->width()/2;
        y = y-image->height()/2;
        QString r = QString::number(image->real(xx, yy), 'f', 2);
        QString i = QString::number(image->imag(xx, yy), 'f', 2);
        value.append("<span>Real: %1, Imaginary: %2</span>");

        value = value.arg(r).arg(i);
    }

    message = message.arg(x).arg(y).arg(value);

    ui->cursorLabel->setText(message);

    // update zoom pixels
    ui->zoomWidget->setPosition(x, y);
}

void ImageViewerWindow::on_mouseClick()
{
    if(_colorPickHandler)
        _colorPickHandler->pickColor(IPLColor(_currentColor.redF(), _currentColor.greenF(), _currentColor.blueF()));

    if(_coordinatePickHandler)
        _coordinatePickHandler->pickCoordinates(_currentPosition.x(), _currentPosition.y());
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_mouseDoubleClick
Doubleclick the image to lock or unlock the position of the zoomWidget.
The position stays the same for all image tabs.
*/
void ImageViewerWindow::on_mouseDoubleClick()
{
    ui->zoomWidget->togglePositionLocked();

    // update icon
    if(ui->zoomWidget->isPositionLocked())
        ui->positionLockedLabel->setStyleSheet("background-image: url(:/padlock_closed_inv.png) no-repeat;");
    else
        ui->positionLockedLabel->setStyleSheet("background-image: url(:/padlock_open_inv.png) no-repeat;");
}
//-----------------------------------------------------------------------------
/*!
ImageViewerWindow::on_zoomWidgetModeCombobox_currentIndexChanged
User selectable options: 3x3, 5x5, 9x9, 17x17, 35x35
*/
void ImageViewerWindow::on_zoomWidgetModeCombobox_currentIndexChanged(int index)
{
    int offset = 1;
    if(index == 1)
        offset = 2;
    else if(index == 2)
        offset = 4;
    else if(index == 3)
        offset = 8;
    else if(index == 4)
        offset = 17;
    ui->zoomWidget->setColumnOffset(offset);
}
