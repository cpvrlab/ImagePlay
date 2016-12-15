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
    addInput("Grayscale Image", IPL_IMAGE_GRAYSCALE);
    addOutput("Complex Image", IPL_IMAGE_COMPLEX);

    // properties
    addProcessPropertyInt("mode", "Windowing Function:None|Hanning|Hamming|Blackman|Border", "", 0, IPL_WIDGET_RADIOBUTTONS);
}

void IPLFFT::destroy()
{
    delete _result;
}

bool IPLFFT::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
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

