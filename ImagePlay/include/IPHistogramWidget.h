//#############################################################################
//  File:      AboutWindow.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPHISTOGRAMWIDGET_H
#define IPHISTOGRAMWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPainter>
#include <QFrame>
#include <QMouseEvent>
#include <QBitmap>
#include <QDebug>
#include <QElapsedTimer>
#include <QApplication>

#include "IPL_processes.h"
#include "IPHistogramThread.h"

//-----------------------------------------------------------------------------
//!IPHistogramWidget calculates and displays a histogram
/*!Supported image types: BW, Grayscale, RGB
*/
class IPHistogramWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IPHistogramWidget(QWidget *parent = 0);

    void updateHistogram(IPLImage* image);
    void setLogarithmic(bool logarithmic);
    IPLData::IPLDataType type()   { return _type; }
    IPLHistogram* histogram()       { return _histogram; }
    IPLHistogram* histogramR()      { return _histogramR; }
    IPLHistogram* histogramG()      { return _histogramG; }
    IPLHistogram* histogramB()      { return _histogramB; }
signals:
    void highlightChangedGrayscale(int, int);
    void highlightChangedColor(int, int, int, int);
    void resetHighlightValue();

public slots:

private:
    int                     _bins;
    int                     _planes;
    bool                    _logarithmic;
    IPLData::IPLDataType  _type;
    IPLHistogram*           _histogram;
    IPLHistogram*           _histogramR;
    IPLHistogram*           _histogramG;
    IPLHistogram*           _histogramB;
    int                     _hightlightPosition;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void leaveEvent(QEvent *);
};

#endif // IPHISTOGRAMWIDGET_H
