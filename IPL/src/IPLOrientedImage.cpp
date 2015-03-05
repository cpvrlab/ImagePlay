#include "IPLOrientedImage.h"

#include "IPLImage.h"

IPLOrientedImage::IPLOrientedImage() : IPLImage()
{
    _type = IPLImage::IMAGE_ORIENTED;
}


IPLOrientedImage::IPLOrientedImage( int width, int height )
{
    _type = IPLImage::IMAGE_ORIENTED;
    _width = width;
    _height = height;
    _rgb32 = NULL;

    // 2 planes are needed for magnitude and phase
    _planes.push_back(new IPLImagePlane( width, height ));
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
