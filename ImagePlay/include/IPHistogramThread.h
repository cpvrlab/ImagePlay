//#############################################################################
//  File:      IPHistogramThread.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPHISTOGRAMTHREAD_H
#define IPHISTOGRAMTHREAD_H
#include <QThread>
#include <QDebug>

#include "IPLHistogram.h"

//-----------------------------------------------------------------------------
//!QThread which calculates the image histogram for the ImageViewerWindow
class IPHistogramThread : public QThread
{
    Q_OBJECT
public:
    IPHistogramThread(IPLHistogram*& histogram,IPLImagePlane* plane, int bins, int range)
    {
        _plane = plane;
        _bins  = bins;
        _range = range;
        _histogram = histogram;
    }

    void run()
    {
        try
        {
            delete _histogram;
            _histogram = new IPLHistogram(_plane, 256, 100);
        }
        catch(std::exception &e)
        {
            qDebug() << "Error: " << e.what();
        }
        catch(...)
        {
            qDebug() << "UNKNOWN ERROR IN THREAD";
        }
    }

private:
    IPLHistogram*   _histogram;
    IPLImagePlane*  _plane;
    int             _bins;
    int             _range;
};

#endif // IPHISTOGRAMTHREAD_H
