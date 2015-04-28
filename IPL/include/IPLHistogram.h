//#############################################################################
//
//  File:      IPLHistogram.h
//  Author:    Kaspar Schmid
//  Date:      November 2014
//
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
#ifndef IPLHISTOGRAM_H
#define IPLHISTOGRAM_H

#include "IPLImagePlane.h"

#include <math.h>
#include <vector>

/**
 * @brief The IPLHistogram class
 */
class IPLHistogram
{
public:
    IPLHistogram(IPLImagePlane* plane, int bins, int range)
    {
        _plane = plane;
        _bins = bins;
        _range = range;

        _histogram.clear();

        // statistics
        _minLevel = 255;
        _maxLevel = 0;
        _meanLevel = 0;
        _medianLevel = 0;
        _modeLevel = 0;
        _totalValues = 0;

        // normalize to range
        _maxCount    = 0;
        _maxCountLog = 0;

        long total = 0;

        if(_plane->height()*_plane->width() == 0)
            return;

        // create bins
        for(int i=0; i<_bins; i++)
        {
            _histogram.push_back(0);
        }

        // collect data
        for(int y=0; y<plane->height(); y++)
        {
            for(int x=0; x<plane->width(); x++)
            {
                unsigned char value = plane->p(x,y) * FACTOR_TO_UCHAR;
                int i = (value * _bins / 256);
                if(i<_bins)
                    _histogram[i]++;

                total += value;
            }
        }

        // collect statistics
        _meanLevel = (float) total / (float) (plane->width()*plane->height());
        for(uint i=0; i < _histogram.size(); i++)
        {
            // mode
            _totalValues += _histogram[i];
            if(_histogram[i] > _maxCount)
            {
                _maxCount = _histogram[i];
                _modeLevel = i;
            }
            _maxCount = std::max(_maxCount, _histogram[i]);

            // min and max
            if(_histogram[i] > 0)
            {
                if(i < _minLevel)
                    _minLevel = i;
                if(i > _maxLevel)
                    _maxLevel = i;
            }
        }

        // median
        int count = 0;
        while( (count <= _totalValues*0.5) && (_medianLevel < _histogram.size()-1) )
            count += _histogram[_medianLevel++];

        // logarithmic scale
        std::vector<double> histogramLogTmp;

        for(uint i=0; i < _histogram.size(); i++)
        {
            // prevent undefined results of log(0)
            double value = std::max(0.0, log((double)_histogram[i]));
            histogramLogTmp.push_back(value);
            _maxCountLog = std::max(_maxCountLog, histogramLogTmp[i]);
        }

        // normalize to range
        for(uint i=0; i < _histogram.size(); i++)
        {
            // normalize to range
            _histogramLinear.push_back(_range*_histogram[i]/_maxCount);

            // normalize and convert to int
            _histogramLog.push_back((int) _range*histogramLogTmp[i]/_maxCountLog);
        }

    }

    float meanLevel()                   { return _meanLevel; }
    float medianLevel()                 { return _medianLevel; }
    unsigned int modeLevel()            { return _modeLevel; }
    unsigned int minLevel()             { return _minLevel; }
    unsigned int maxLevel()             { return _maxLevel; }
    unsigned int maxValue()             { return _maxCount; }
    std::vector<int>    linear()        { return _histogramLinear; }
    std::vector<int>    logarithmic()   { return _histogramLog; }

    int rawValueAt(int level)
    {
        return _histogram.at(level);
    }

    float percentageAt(int level)
    {
        return 100.0 * _histogram.at(level) / _totalValues;
    }

private:
    IPLImagePlane*      _plane;
    int                 _bins;
    int                 _range;
    unsigned int        _totalValues;
    unsigned int        _minLevel;
    unsigned int        _maxLevel;
    float               _meanLevel;
    float               _medianLevel;
    unsigned int        _modeLevel;
    int                 _maxCount;
    double              _maxCountLog;
    std::vector<int>    _histogram;         //!< raw count
    std::vector<int>    _histogramLinear;   //!< normalized to range
    std::vector<int>    _histogramLog;      //!< normalized to range with logarithmic scale
};

#endif // IPLHISTOGRAM_H
