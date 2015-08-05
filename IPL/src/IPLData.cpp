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

#include "IPLData.h"

#include "IPLImage.h"
#include "IPLComplexImage.h"
#include "IPLPoint.h"
#include "IPLMatrix.h"


bool IPLData::isConvertibleTo(IPLDataType dataType)
{
    switch (dataType)
    {
    case IPLData::IMAGE_BW:
    case IPLData::IMAGE_GRAYSCALE:
    case IPLData::IMAGE_COLOR:
        return toImage() != NULL;
    case IPLData::IMAGE_COMPLEX:
        return toComplexImage() != NULL;
    case IPLData::POINT:
        return toPoint() != NULL;
    case IPLData::MATRIX:
        return toMatrix() != NULL;
    case IPLData::IMAGE_ORIENTED:
    case IPLData::SHAPES:
    case IPLData::UNDEFINED:
    default:
        throw std::logic_error("Unknown method for data type");
        break;
    }
    return false;
}

IPLImage* IPLData::toImage()
{
    return dynamic_cast<IPLImage*>(this);
}

IPLComplexImage* IPLData::toComplexImage()
{
    return dynamic_cast<IPLComplexImage*>(this);
}

IPLPoint* IPLData::toPoint()
{
    return dynamic_cast<IPLPoint*>(this);
}

IPLMatrix* IPLData::toMatrix()
{
    return dynamic_cast<IPLMatrix*>(this);
}
