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
#include <QScopedPointer>

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
    IPLDataType type()   { return _type; }
    //TODO: Use references?
    IPLHistogram* histogram()       { return _histogram.data(); }
    IPLHistogram* histogramR()      { return _histogramR.data(); }
    IPLHistogram* histogramG()      { return _histogramG.data(); }
    IPLHistogram* histogramB()      { return _histogramB.data(); }
signals:
    void highlightChangedGrayscale(int, int, float);
    void highlightChangedColor(int, int, int, int, float, float, float);
    void resetHighlightValue();

public slots:

private:
    int                          _bins;
    int                          _planes;
    bool                         _logarithmic;
    IPLDataType         _type;
    QScopedPointer<IPLHistogram> _histogram;
    QScopedPointer<IPLHistogram> _histogramR;
    QScopedPointer<IPLHistogram> _histogramG;
    QScopedPointer<IPLHistogram> _histogramB;
    int                          _hightlightPosition;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void leaveEvent(QEvent *);
};

#endif // IPHISTOGRAMWIDGET_H
