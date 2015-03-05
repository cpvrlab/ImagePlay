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

IPLImagePlane::~IPLImagePlane( void )
{
    deletePlane();

    _instanceCount--;
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
