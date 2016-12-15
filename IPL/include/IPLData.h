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

#ifndef IPLDATA_H
#define IPLDATA_H

#include "IPL_global.h"

#include <string>

class IPLImage;
class IPLComplexImage;
class IPLPoint;
class IPLMatrix;
class IPLKeyPoints;

class IPLSHARED_EXPORT IPLData
{
public:
    IPLData             ()                      { _type = IPL_UNDEFINED; }
    IPLData             (IPLDataType type)      { _type = type; }
    virtual             ~IPLData()                              {}
    IPLDataType         type            (void)                  { return _type; }

    bool                isConvertibleTo(IPLDataType);
    IPLImage*           toImage();
    IPLComplexImage*    toComplexImage();
    IPLPoint*           toPoint();
    IPLMatrix*          toMatrix();
    IPLKeyPoints*       toKeyPoints();

protected:
    IPLDataType         _type;
};

#endif // IPLDATA_H
