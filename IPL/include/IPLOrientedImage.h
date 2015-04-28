//#############################################################################
//
//  File:      IPLOrientedImage.h
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

#ifndef IPLORIENTEDIMAGE_H
#define IPLORIENTEDIMAGE_H

#include "IPL_global.h"

#include "IPLImage.h"

/**
 * @brief The IPLComplexImage class
 */
class IPLSHARED_EXPORT IPLOrientedImage : public IPLImage
{
public:
    IPLOrientedImage();
    IPLOrientedImage( const IPLImage& image );
    IPLOrientedImage( int width, int height );
    ~IPLOrientedImage();

    ipl_basetype& magnitude(int x, int y);
    ipl_basetype& phase(int x, int y);
};

#endif // IPLORIENTEDIMAGE_H
