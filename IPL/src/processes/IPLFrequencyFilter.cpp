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

#include "IPLFrequencyFilter.h"

void IPLFrequencyFilter::init()
{
    // init
    _result     = NULL;
    _filter     = NULL;

    // basic settings
    setClassName("IPLFrequencyFilter");
    setTitle("Frequency Filter");
    setCategory(IPLProcess::CATEGORY_FOURIER);
    setDescription("Inverse Fast Fourier Transform.");
    setKeywords("FFT");

    // inputs and outputs
    addInput("Complex Image", IPL_IMAGE_COMPLEX);
    addOutput("Complex Image", IPL_IMAGE_COMPLEX);
    //addOutput("Filter", IPL_IMAGE_GRAYSCALE);

    // properties
    addProcessPropertyInt("maskType", "Mask Type:Low Pass|High Pass|Band Pass|Band Stop", "low|high|*|*", 0, IPL_WIDGET_GROUP);
    addProcessPropertyDouble("low_cutoff", "Low Cutoff", "", 0.3, IPL_WIDGET_SLIDER, 0.0, 1.0);
    addProcessPropertyDouble("high_cutoff", "High Cutoff", "", 0.6, IPL_WIDGET_SLIDER, 0.0, 1.0);
    addProcessPropertyBool("keepDC", "Keep DC", "", false, IPL_WIDGET_CHECKBOXES);
}

void IPLFrequencyFilter::destroy()
{
    delete _result;
}

bool IPLFrequencyFilter::processInputData(IPLImage* data , int, bool)
{
    IPLComplexImage* complexImageData = data->toComplexImage();
    if (NULL == complexImageData) {
        // TODO write an error message
        return false;
    }
    _input = new IPLComplexImage(*complexImageData);

    // delete previous result
    delete _result;
    delete _filter;

    _result = new IPLComplexImage(*_input);
    int width = _result->width();
    int height = _result->height();
    _filter = new IPLImage(IPL_IMAGE_GRAYSCALE, width, height);


    // get properties
    int maskType = getProcessPropertyInt("maskType");
    double lowCutoff = getProcessPropertyDouble("low_cutoff");
    double highCutoff = getProcessPropertyDouble("high_cutoff");
    bool keepDC = getProcessPropertyBool("keepDC");

    int halfSize = _result->width()/2;
    int size = _result->width();
    int lowRange = lowCutoff*lowCutoff*halfSize*halfSize;
    int highRange = highCutoff*highCutoff*halfSize*halfSize;

    Complex c0 = Complex( 0.0, 0.0 );

    for( int x=-halfSize; x<halfSize; x++ )
    {
        for( int y=-halfSize; y<halfSize; y++ )
        {
            int X = ( x < 0 )? size+x : x;
            int Y = ( y < 0 )? size+y : y;
            int range = x*x+y*y;
            switch( maskType )
            {
                case 0: if( range > lowRange )
                            _result->c(X,Y) = c0;
                        break;
                case 1: if( range < highRange )
                            _result->c(X,Y) = c0;
                        break;
                case 2: if( range < lowRange || range > highRange )
                            _result->c(X,Y) = c0;
                        break;
                case 3: if( range > lowRange && range < highRange )
                            _result->c(X,Y) = c0;
                        break;
            }
        }
    }

    if( keepDC ) _result->c( 0, 0 ) = _input->c( 0, 0 );

    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            if(_result->real(x,y) > 0.0)
                _filter->plane(0)->p(x,y) = 1.0;
            else
                _filter->plane(0)->p(x,y) = 0.0;
        }
    }

    return true;
}

IPLData* IPLFrequencyFilter::getResultData(int)
{
    return _result;
}
