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

#include "IPLImagePlane.h"

ipl_basetype IPLImagePlane::_zero = 0.0f;

int IPLImagePlane::_instanceCount = 0;

IPLImagePlane::IPLImagePlane(void)
{
    _height = 0;
    _width = 0;
    _plane = NULL;

    _instanceCount++;
}

IPLImagePlane::IPLImagePlane( int w, int h )
{
    _height = h;
    _width = w;
    newPlane();

    _instanceCount++;
}

IPLImagePlane::IPLImagePlane( const IPLImagePlane& other )
{
    if( this != &other )
    {
        _height = other._height;
        _width = other._width;
        newPlane();

        for(int i=0; i<_height*_width; i++)
            _plane[i] = other._plane[i];
    }

    _instanceCount++;
}

IPLImagePlane::IPLImagePlane(IPLImagePlane &&other):
    _height(other._height),
    _width(other._width),
    _plane(other._plane)
{
    other._height = 0;
    other._width = 0;
    other._plane = NULL;
    _instanceCount++;
}

IPLImagePlane::~IPLImagePlane( void )
{
    deletePlane();

    _instanceCount--;
}

IPLImagePlane &IPLImagePlane::operator=(const IPLImagePlane &other)
{
    _height = other._height;
    _width = other._width;
    newPlane();

    for(int i=0; i<_height*_width; i++)
        _plane[i] = other._plane[i];

    return *this;
}

IPLImagePlane &IPLImagePlane::operator=(IPLImagePlane &&other)
{
    _height = other._height;
    _width = other._width;
    _plane = other._plane;

    other._height = 0;
    other._width = 0;
    other._plane = NULL;

    return *this;
}


void IPLImagePlane::swap(IPLImagePlane &other)
{
    IPLImagePlane tmp(std::move(other));

    other = std::move(*this);
    (*this) = std::move(tmp);
}

void IPLImagePlane::newPlane( void )
{
    // automatically init to 0
    _plane = new ipl_basetype[_height * _width]();
}

void IPLImagePlane::deletePlane( void )
{
    delete [] _plane;
}
