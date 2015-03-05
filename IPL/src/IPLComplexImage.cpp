#include "IPLComplexImage.h"


IPLComplexImage::IPLComplexImage() : IPLImage()
{
}


IPLComplexImage::IPLComplexImage( int width, int height )
{
    _width = width;
    _height = height;
    _rgb32 = NULL;

    // 2 planes are needed for real and imaginary part
    _planes.push_back(new IPLImagePlane( width, height ));
    _planes.push_back(new IPLImagePlane( width, height ));

    fillColor(0.0);
}

IPLComplexImage::~IPLComplexImage()
{

}

ipl_basetype IPLComplexImage::real(int x, int y)
{
    return plane(0)->p(x, y);
}

ipl_basetype IPLComplexImage::imag(int x, int y)
{
    return plane(1)->p(x, y);
}
