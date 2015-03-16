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

ipl_basetype& IPLComplexImage::real(int x, int y)
{
    return plane(0)->p(x, y);
}

ipl_basetype& IPLComplexImage::imag(int x, int y)
{
    return plane(1)->p(x, y);
}


void IPLComplexImage::flip( void )
{/*
    Complex** tempPlane = plane;
    int h =height;
    height = width;
    width = h;
    newPlane();
    for( int y=0; y<height; y++ )
    {
        for( int x=0; x<width; x++ )
        {
            p(x,y) = (tempPlane[x])[y];
        }
    }
    for(int y=0; y<width; y++)
        delete [] tempPlane[y];
    delete [] tempPlane;*/
}

Complex* IPLComplexImage::getRow( int y )
{
    return _plane[y];
}

int IPLComplexImage::nextPowerOf2( int x )
{
    int result = 1;
    while(result < x)
        result <<= 1;
    return result;
}

void IPLComplexImage::Twiddle( Complex* x, int N)
{
    double pi2 = PI / (double) N;
    for( int i=0; i<N; i++ )
    {
        double omega = pi2 * (double) i;
        x[i] = Complex( cos(omega), sin(omega) );
    }
}

void IPLComplexImage::iTwiddle( Complex* x, int N)
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
    if( _width != _height ) return false;
    int size = IPLComplexImage::nextPowerOf2(_width);
    if( size != _width ) return false;

    Complex* twiddle = new Complex [size/2];
    Twiddle( twiddle, size/2 );
    for( int y=0; y<size; y++ )
    {
        lineFFT( getRow(y), twiddle, size);
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
        lineFFT( getRow(y), twiddle, size);
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
        lineFFT( getRow(y), twiddle, size);
    }
    flip();
    for( int y=0; y<size; y++ )
    {
        lineFFT( getRow(y), twiddle, size);
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
Complex temp, w; int m,           /* stage of computation */
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
