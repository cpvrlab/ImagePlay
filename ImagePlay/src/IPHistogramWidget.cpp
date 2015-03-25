#include "IPHistogramWidget.h"

IPHistogramWidget::IPHistogramWidget(QWidget *parent) :
    QWidget(parent)
{
    _bins = 0;
    _planes = 0;
    _logarithmic = false;
    _hightlightPosition = -1;

    _histogram.reset(NULL);
    _histogramR.reset(NULL);
    _histogramG.reset(NULL);
    _histogramB.reset(NULL);

    _type = IPLData::UNDEFINED;

    setCursor(Qt::BlankCursor);
    setMouseTracking(true);
}

void IPHistogramWidget::updateHistogram(IPLImage *image)
{
    if(!image)
    {
        _type = IPLData::UNDEFINED;
        update();
        return;
    }

    // only update if the panel is not hidden
    if(isVisible())
    {
        _type = image->type();

        QElapsedTimer timer;
        timer.start();
        if(_type == IPLImage::IMAGE_BW)
        {
            _histogram.reset(new IPLHistogram(image->plane(0), 2, 100));
        }
        else if(_type == IPLData::IMAGE_GRAYSCALE)
        {
            _histogram.reset(new IPLHistogram(image->plane(0), 256, 100));
        }
        else if(_type == IPLData::IMAGE_COLOR)
        {
            _histogramR.reset(new IPLHistogram(image->plane(0), 256, 100));
            _histogramG.reset(new IPLHistogram(image->plane(1), 256, 100));
            _histogramB.reset(new IPLHistogram(image->plane(2), 256, 100));
        }

        // repaint
        update();
    }
}

void IPHistogramWidget::setLogarithmic(bool logarithmic)
{
    _logarithmic = logarithmic;
    update();
}

void IPHistogramWidget::paintEvent(QPaintEvent* e)
{
    // somewhere in this method we have a crash :(

    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing);

    QPen penWhite(Qt::white);

    // fill background
    QBrush background(QColor(64,64,64));
    painter.fillRect(0,0,256,100,background);

    if(_type == IPLData::UNDEFINED)
        return;

    // COLOR
    if(_type == IPLData::IMAGE_COLOR)
    {
//        painter.setCompositionMode(QPainter::CompositionMode_Screen);

        QPen penR(QColor(255,0,0,100));
        QPen penG(QColor(0,255,0,100));
        QPen penB(QColor(0,0,255,100));
        QPen penR2(QColor(255,0,0));
        QPen penG2(QColor(0,255,0));
        QPen penB2(QColor(0,0,255));

        std::vector<int> valuesR = _logarithmic ? _histogramR->logarithmic() : _histogramR->linear();
        std::vector<int> valuesG = _logarithmic ? _histogramG->logarithmic() : _histogramG->linear();
        std::vector<int> valuesB = _logarithmic ? _histogramB->logarithmic() : _histogramB->linear();
        for(uint i=0; i < valuesR.size(); i++)
        {
            // R
            painter.setPen(penR);
            painter.drawLine(i,100,i,100-valuesR[i]);
            painter.setPen(penR2);
            painter.drawPoint(i,100-valuesR[i]);

            // G
            painter.setPen(penG);
            painter.drawLine(i,100,i,100-valuesG[i]);
            painter.setPen(penG2);
            painter.drawPoint(i,100-valuesG[i]);

            // B
            painter.setPen(penB);
            painter.drawLine(i,100,i,100-valuesB[i]);
            painter.setPen(penB2);
            painter.drawPoint(i,100-valuesB[i]);
        }

        if(_hightlightPosition > -1)
        {
            //int valueMax = std::max(valuesR[_hightlightPosition], std::max(valuesG[_hightlightPosition], valuesB[_hightlightPosition]));
            painter.setPen(penWhite);
            painter.setCompositionMode(QPainter::CompositionMode_Difference);
            painter.drawLine(_hightlightPosition,0,_hightlightPosition,100);
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        }
    }
    else if(_type == IPLData::IMAGE_GRAYSCALE)
    {
        std::vector<int> values = _logarithmic ? _histogram->logarithmic() : _histogram->linear();

        QPen pen(QColor(255,255,255,100));
        QPen pen2(QColor(255,255,255));
        for(size_t i=0; i < values.size(); i++)
        {
            painter.setPen(pen);
            painter.drawLine(i,100,i,100-values[i]);
            painter.setPen(pen2);
            painter.drawPoint(i,100-values[i]);
        }

        if(_hightlightPosition > -1)
        {
            painter.setPen(penWhite);
            painter.setCompositionMode(QPainter::CompositionMode_Difference);
            painter.drawLine(_hightlightPosition,0,_hightlightPosition,100);
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        }
    }
    else if(_type == IPLImage::IMAGE_BW)
    {
        std::vector<int> values = _logarithmic ? _histogram->logarithmic() : _histogram->linear();
        painter.fillRect(0,100-values[0],127,values[0], Qt::gray);
        painter.fillRect(127,100-values[1],128,values[1], Qt::gray);

        if(_hightlightPosition > -1)
        {
            painter.setPen(penWhite);
            painter.setCompositionMode(QPainter::CompositionMode_Difference);
            painter.drawLine(_hightlightPosition,0,_hightlightPosition,100);
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        }
    }

    QWidget::paintEvent(e);
}


void IPHistogramWidget::mouseMoveEvent(QMouseEvent* event)
{
    _hightlightPosition = event->pos().x();
    update();

    if(_type == IPLData::IMAGE_COLOR)
    {
        int valueR = _histogramR->rawValueAt(_hightlightPosition);
        int valueG = _histogramG->rawValueAt(_hightlightPosition);
        int valueB = _histogramB->rawValueAt(_hightlightPosition);
        float percentageR = _histogramR->percentageAt(_hightlightPosition);
        float percentageG = _histogramG->percentageAt(_hightlightPosition);
        float percentageB = _histogramB->percentageAt(_hightlightPosition);

        emit highlightChangedColor(_hightlightPosition, valueR, valueG, valueB, percentageR, percentageG, percentageB);
    }
    else if(_type == IPLData::IMAGE_GRAYSCALE)
    {
        int value = _histogram->rawValueAt(_hightlightPosition);
        float percentage = _histogram->percentageAt(_hightlightPosition);

        emit highlightChangedGrayscale(_hightlightPosition, value, percentage);
    }
    else if(_type == IPLData::IMAGE_BW)
    {
        int pos = _hightlightPosition < width()/2 ? 0 : 1;
        int value = _histogram->rawValueAt(pos);
        int percentage = _histogram->percentageAt(pos);

        emit highlightChangedGrayscale(pos, value, percentage);
    }
}

void IPHistogramWidget::leaveEvent(QEvent *)
{
    // hide highlight
    _hightlightPosition = -1;
    update();

    emit resetHighlightValue();
}
