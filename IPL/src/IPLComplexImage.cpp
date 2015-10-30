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

#include "IPLComplexImage.h"

IPLComplexImage::IPLComplexImage() : IPLData()
{
    _plane = NULL;
    _type = IPL_IMAGE_COMPLEX;
}

IPLComplexImage::IPLComplexImage(const IPLComplexImage &other):
    IPLData(other)
{
    if(this != &other)
    {
        _height = other._height;
        _width = other._width;
        _type = other._type;
        _rgb32.resize(_height * _width * 4);

        newPlane();

        for( int y=0; y<_height; y++)
            for( int x=0; x<_width; x++)
                _plane[x][y] = other._plane[x][y];
    }
}


IPLComplexImage::IPLComplexImage( int width, int height )
{
    _plane = NULL;
    _width = width;
    _height = height;
    _rgb32.resize(_height * _width*4);
    _type = IPL_IMAGE_COMPLEX;
    newPlane();
}

IPLComplexImage::~IPLComplexImage()
{

}

void IPLComplexImage::newPlane(void)
{
    _plane = new Complex * [_height];

    for(int y=0; y<_height; y++)
        _plane[y] = new Complex [_width];
}
/*!
 * \brief IPLComplexImage::rgb32
 * \param mode: 0 = REAL, 1 = IMAG
 * \return
 */
unsigned char* IPLComplexImage::rgb32(int mode/* = 0*/)
{
    _mutex.lock();

    // scale from min to max
    float min = FLT_MAX;
    float max = FLT_MIN;
    for(int y=0; y < _height; y++)
    {
        for(int x=0; x < _width; x++)
        {
            float value;
            if(mode == 0)
                value = std::abs(real(x,y));
            else
                value = std::abs(imag(x,y));

            if(value < min)
                min = value;
            if(value > max)
                max = value;
        }
    }
    const double delta = 0.00001;
    const double logdelta = log( delta );
    max = (max!=0.0)? log(delta+max) : logdelta;
    min = (min!=0.0)? log(delta+min) : logdelta;
    double scale = (max-min)? 255.0 / (max-min) : 1.0;

    // generate rgb32
    int i=0;
    for(int y=0; y < _height; y++)
    {
        for(int x=0; x < _width; x++)
        {
            // move 0/0 to the center for better visualization
            int xx = ((x-_width/2) + _width) % _width;
            int yy = ((y-_height/2) + _height) % _height;
            double c;
            if(mode == 0)
                c = std::abs(real(xx,yy));
            else
                c = std::abs(imag(xx,yy));
            double lgc = (c!=0.0)? log(delta+c) : logdelta;
            uchar val = ( (lgc-min)*scale );
            _rgb32[i++] = val;
            _rgb32[i++] = val;
            _rgb32[i++] = val;
            _rgb32[i++] = 0xFF;
        }
    }
    _mutex.unlock();

    return _rgb32.data();
}

Complex& IPLComplexImage::c(int x, int y)
{
    return _plane[x][y];
}

ipl_basetype IPLComplexImage::real(int x, int y)
{
    return (ipl_basetype) _plane[x][y].real();
}

ipl_basetype IPLComplexImage::imag(int x, int y)
{
    return (ipl_basetype) _plane[x][y].imag();
}

ipl_basetype IPLComplexImage::maxReal()
{
    float max = FLT_MIN;
    for(int y=0; y < _height; y++)
    {
        for(int x=0; x < _width; x++)
        {
            if(std::abs(real(x,y)) > max)
                max = real(x,y);
        }
    }
    return max;
}

ipl_basetype IPLComplexImage::minReal()
{
    float min = FLT_MAX;
    for(int y=0; y < _height; y++)
    {
        for(int x=0; x < _width; x++)
        {
            if(std::abs(real(x,y)) < min)
                min = real(x,y);
        }
    }
    return min;
}

void IPLComplexImage::flip(void)
{
    Complex** tempPlane = _plane;
    int h = _height;
    _height = _width;
    _width = h;
    newPlane();
    for( int y=0; y<_height; y++ )
    {
        for( int x=0; x<_width; x++ )
        {
            c(x,y) = (tempPlane[y])[x];
        }
    }
    for(int y=0; y<_width; y++)
        delete [] tempPlane[y];
    delete [] tempPlane;
}

int IPLComplexImage::nextPowerOf2( int x )
{
    int result = 1;
    while(result < x)
        result <<= 1;
    return result;
}

void IPLComplexImage::Twiddle(Complex* x, int N)
{
    double pi2 = PI / (double) N;
    for( int i=0; i<N; i++ )
    {
        double omega = pi2 * (double) i;
        x[i] = Complex( cos(omega), sin(omega) );
    }
}

void IPLComplexImage::iTwiddle(Complex* x, int N)
{
    double pi2 = - PI / (double) N;
    for( int i=0; i<N; i++ )
    {
        double omega = pi2 * (double) i;
        x[i] = Complex( cos(omega), sin(omega) );
    }
}

bool IPLComplexImage::FFT()
{
    if(_width != _height) return false;
    int size = IPLComplexImage::nextPowerOf2(_width);
    if(size != _width) return false;

    Complex* twiddle = new Complex [size/2];
    Twiddle( twiddle, size/2 );
    for( int y=0; y<size; y++ )
    {
        lineFFT(_plane[y], twiddle, size);
    }
/*    for( int y=1; y<size; y++ )
        for( int x=0; x<size; x++ )
            p(x,0) += p(x,y);
    for( int y=1; y<size; y++ )
        for( int x=0; x<size; x++ )
            p(x,y) = p(x,0);  */
    flip();
    for( int y=0; y<size; y++ )
    {
        lineFFT(_plane[y], twiddle, size);
    }
    delete [] twiddle;
    return true;
}

bool IPLComplexImage::IFFT()
{
    if( _width != _height ) return false;
    int size = IPLComplexImage::nextPowerOf2(_width);
    if( size != _width ) return false;

    Complex* twiddle = new Complex [size/2];
    iTwiddle( twiddle, size/2 );
    for( int y=0; y<size; y++ )
    {
        lineFFT(_plane[y], twiddle, size);
    }
    flip();
    for( int y=0; y<size; y++ )
    {
        lineFFT(_plane[y], twiddle, size);
    }
    /*delete [] twiddle;
    double height_2 = (double)_height * (double)_height;
    for( int x=0; x<size; x++ )
        for( int y=0; y<size; y++ )
            p(x,y) /= height_2;*/
    return true;
}

void IPLComplexImage::lineFFT(Complex* x, Complex* twiddle, int N)
{
Complex temp, w;
int m,           /* stage of computation */
    step,        /* step between indexes p and q */
    i,           /* index of Xm(p) in DFT */
    p, q,        /* indexes of butterfly values */
    r;           /* power of Wn */

    /* bit-reversal ordering in place */

    p = 0;
    for(i=0; i < N-1; i++)
    {
        if (i<p)
        {
            temp = x[i];
            x[i] = x[p];
            x[p] = temp;
        }
        q = N/2;
        while (q <= p)
        {
            p -= q;
            q /= 2;
        }
        p += q;
    }

    /* decimation in time algorithm */

    m = 1;
    while (m < N)
    {
        step = 2 * m;
        for(i=0; i<m; i++)
        {
            r = i * (N/2 / m);
            w = twiddle[r];
            p = i;
            do
            {
                 q = p + m;
                 /* butterfly */
                 temp = w * x[q];
                 x[q] = x[p] - temp;
                 x[p] += temp;
                 p += step;
             } while (p < N);
        }
        m = step;
    }
}
