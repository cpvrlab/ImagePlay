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

#include "IPLOrientedImage.h"

#include "IPLImage.h"

IPLOrientedImage::IPLOrientedImage() : IPLImage()
{
    _type = IPLImage::IMAGE_ORIENTED;
}


IPLOrientedImage::IPLOrientedImage( int width, int height )
  : IPLImage( IPLImage::IMAGE_ORIENTED, width, height )
{
    // 2 planes are needed for magnitude and phase
    // 1 plane already created by default constructor
    _planes.push_back(new IPLImagePlane( width, height ));
    _nrOfPlanes = 2;

    fillColor(0.0);
}

IPLOrientedImage::~IPLOrientedImage()
{

}

ipl_basetype& IPLOrientedImage::magnitude(int x, int y)
{
    return this->plane(0)->p(x, y);
}

ipl_basetype& IPLOrientedImage::phase(int x, int y)
{
    return this->plane(1)->p(x, y);
}
