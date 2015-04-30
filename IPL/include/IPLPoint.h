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

#ifndef IPLPOINT_H
#define IPLPOINT_H

#include "IPL_global.h"

#include "IPLData.h"

#include <sstream>

/**
 * @brief The IPLPoint class
 */
class IPLSHARED_EXPORT IPLPoint : public IPLData
{
public:
    IPLPoint();
    IPLPoint(double x, double y);
    IPLPoint(const IPLPoint& other);
    IPLPoint& operator=(const IPLPoint& other);
    ~IPLPoint();

    void setX(double value)       { _x = value; }
    void setY(double value)       { _y = value; }

    double x() const  { return _x; }
    double y() const  { return _y; }

    std::string toString(int x, int y);
private:
    double _x;
    double _y;
};


#endif // IPLPOINT_H
