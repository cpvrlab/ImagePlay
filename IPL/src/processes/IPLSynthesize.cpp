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

#include "IPLSynthesize.h"

void IPLSynthesize::init()
{
    // init
    _result     = NULL;
    _type       = 0;
    _width      = 512;
    _height     = 512;
    _amplitude  = 0.5f;
    _offset     = 0.5f;
    _wavelength = 8;
    _direction  = 0;
    _decay      = 0;

    // basic settings
    setClassName("IPLSynthesize");
    setTitle("Synthesize Image");
    setCategory(IPLProcess::CATEGORY_IO);
    setIsSource(true);

    // inputs and outputs
    addOutput("Image", IPL_IMAGE_GRAYSCALE);

    // all properties which can later be changed by gui
    addProcessPropertyInt("type", "Type:Flat|Plane Wave|Center Wave","flat|plane|radial", _type, IPL_WIDGET_GROUP);
    addProcessPropertyInt("width", "Width","", _width, IPL_WIDGET_SLIDER, 1, 1024);
    addProcessPropertyInt("height", "Height","", _height, IPL_WIDGET_SLIDER, 1, 1024);
    addProcessPropertyDouble("amplitude", "Amptlitude","", _amplitude, IPL_WIDGET_SLIDER, 0.0f, 1.0f);
    addProcessPropertyDouble("offset", "Offset","", _offset, IPL_WIDGET_SLIDER, 0.0f, 1.0f);
    addProcessPropertyInt("wavelength", "Wavelength","", _wavelength, IPL_WIDGET_SLIDER, 1, 1024);
    addProcessPropertyInt("plane_direction", "Direction","", _direction, IPL_WIDGET_SLIDER, 0, 360);
    addProcessPropertyInt("decay", "Decay","", _decay, IPL_WIDGET_SLIDER, 0, 1024);
    addProcessPropertyColor("flat_color", "Color","", IPLColor(0,0,0), IPL_WIDGET_COLOR_RGB);
}

void IPLSynthesize::destroy()
{
    delete _result;
}

bool IPLSynthesize::processInputData(IPLData*, int, bool)
{
    if(isResultReady())
    {
        //return true;
    }

    // delete previous result
    delete _result;
    _result = NULL;

    // get properties
    _type       = getProcessPropertyInt("type");
    _width      = getProcessPropertyInt("width");
    _height     = getProcessPropertyInt("height");
    _amplitude  = getProcessPropertyDouble("amplitude");
    _offset     = getProcessPropertyDouble("offset");
    _wavelength = getProcessPropertyInt("wavelength");
    _direction  = getProcessPropertyInt("plane_direction");
    _decay      = getProcessPropertyInt("decay");

    IPLColor color = getProcessPropertyColor("flat_color");

    if( _type == 0 )
        _result = new IPLImage( IPL_IMAGE_COLOR, _width, _height );
    else
        _result = new IPLImage( IPL_IMAGE_GRAYSCALE, _width, _height );

    double dx = (double)_width / 2.0;
    double dy = (double)_height / 2.0;
    double direction = _direction / 180.0 * PI; // deg to rad

    IPLImagePlane* plane = _result->plane( 0 );

    int progress = 0;
    int maxProgress = _result->height();

    switch( _type )
    {
    case 0: // flat plane
        for( int y=0; y<_height; y++ )
        {
            notifyProgressEventHandler(100*progress++/maxProgress);
            for( int x=0; x<_width; x++ )
            {
                _result->plane(0)->p(x,y) = color.red();
                _result->plane(1)->p(x,y) = color.green();
                _result->plane(2)->p(x,y) = color.blue();
            }
        }
        break;
    case 1: // plane wave
        for( int y=0; y<_height; y++ )
        {
            notifyProgressEventHandler(100*progress++/maxProgress);
            for( int x=0; x<_width; x++ )
            {
                double dist = (x)*cos( direction ) + (_height-y)*sin( direction );
                double fade = (_decay!=0) ? exp( -dist/_decay ) : 1.0;
                double value = _amplitude * cos(dist/_wavelength * PI * 2.0) * fade + _offset;
                plane->p(x,y) = ( (value<0.0)? 0.0 : (value>1.0)? 1.0 : value );
            }
        }
        break;
    case 2:// center wave
        for( int y=0; y<_height; y++ )
        {
            notifyProgressEventHandler(100*progress++/maxProgress);
            for( int x=0; x<_width; x++ )
            {
                double dist = sqrt( (x-dx)*(x-dx) + (y-dy)*(y-dy) );
                double fade = (_decay!=0) ? exp( -dist/_decay ) : 1.0;
                double value = _amplitude * cos( dist/_wavelength * PI * 2.0 ) * fade + _offset;
                plane->p(x,y) = ( (value<0.0)? 0.0 : (value>1.0)? 1.0 : value );
            }
        }
        break;
    }

    return true;
}

IPLImage *IPLSynthesize::getResultData(int)
{
    return _result;
}
