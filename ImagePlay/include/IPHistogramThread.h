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
