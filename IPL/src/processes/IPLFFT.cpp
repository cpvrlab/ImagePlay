#include "IPLFFT.h"

void IPLFFT::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLFFT");
    setTitle("FFT");
    setCategory(IPLProcess::CATEGORY_FOURIER);
    setDescription("Fast Fourier Transform.");

    // inputs and outputs
    addInput("Grayscale Image", IPLData::IMAGE_GRAYSCALE);
    addOutput("Complex Image", IPLImage::IMAGE_COMPLEX);

    // properties
    addProcessPropertyInt("mode", "Windowing Function:None|Hanning|Hamming|Blackman|Border", "", IPL_INT_RADIOBUTTONS, 0);
}

void IPLFFT::destroy()
{
    delete _result;
}

bool IPLFFT::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    int cWidth = IPLComplexImage::nextPowerOf2(width);
    int cHeight = IPLComplexImage::nextPowerOf2(height);
    int size = cHeight = cWidth = (cWidth>cHeight)? cWidth : cHeight;

    _result = new IPLComplexImage(cWidth, cHeight);

    // get properties
    int mode = getProcessPropertyInt("mode");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();

    // image center
    int dx = ( cWidth - width )/2;
    int dy = ( cHeight - height )/2;

    IPLImagePlane* plane = image->plane(0);
    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);
        for(int x=0; x<width; x++)
        {
            _result->c(x+dx, y+dy) = Complex(plane->p(x,y), 0.0);
        }
    }

    // windowing function
    switch(mode)
    {
        case 0: // rectangular
                break;

        case 1: // Hanning
                for( int y=0; y<size; y++ )
                    for( int x=0; x<size; x++ )
                        _result->c(x,y) *= Hanning(x, size) * Hanning(y, size);
                break;
        case 2: // Hamming
                for( int y=0; y<size; y++ )
                    for( int x=0; x<size; x++ )
                        _result->c(x,y) *= Hamming(x, size) * Hamming(y, size);
                break;
        case 3: // Blackman
                for( int y=0; y<size; y++ )
                    for( int x=0; x<size; x++ )
                        _result->c(x,y) *= Blackman(x, size) * Blackman(y, size);
                break;
        case 4: // Border only
                int border = size / 32;
                for( int y=0; y<border; y++ )
                    for( int x=0; x<size; x++ )
                    {
                        double factor = (0.54 - 0.46 * cos( 2.0 * PI * y / border / 2.0 ));
                        _result->c(x,y) *= factor;
                        _result->c(x,size-y-1) *= factor;
                    }
                for( int x=0; x<border; x++ )
                    for( int y=0; y<size; y++ )
                    {
                        double factor = (0.54 - 0.46 * cos( 2.0 * PI * x / border / 2.0 ));
                        _result->c(x,y) *= factor;
                        _result->c(size-x-1,y) *= factor;
                    }
                break;
    }

    _result->FFT();

    return true;
}

IPLData* IPLFFT::getResultData( int )
{
    return _result;
}


double IPLFFT::Hanning(int n, int size)
{
    return ( 0.5*(1.0 - cos( 2.0 * PI * n / (size-1) ) ) );
}

double IPLFFT::Hamming(int n, int size)
{
    return ( 0.54 - 0.46 * cos( 2.0 * PI * n / (size-1) ) );
}

double IPLFFT::Blackman(int n, int size)
{
    return ( 0.42 - 0.5 * cos( 2.0 * PI * n / (size-1) ) + 0.08 * cos( 4.0 * PI * n / (size-1) ) );
}

