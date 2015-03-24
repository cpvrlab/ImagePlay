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
    addOutput("Image", IPLData::IMAGE_GRAYSCALE);

    // all properties which can later be changed by gui
    addProcessPropertyInt("type", "Type:Plane Wave|Center Wave","", IPL_INT_RADIOBUTTONS, _type);
    addProcessPropertyInt("width", "Width","", IPL_INT_SLIDER, _width, 1, 1024);
    addProcessPropertyInt("height", "Height","", IPL_INT_SLIDER, _height, 1, 1024);
    addProcessPropertyDouble("amplitude", "Amptlitude","", IPL_DOUBLE_SLIDER, _amplitude, 0.0f, 1.0f);
    addProcessPropertyDouble("offset", "Offset","", IPL_DOUBLE_SLIDER, _offset, 0.0f, 1.0f);
    addProcessPropertyInt("wavelength", "Wavelength","", IPL_INT_SLIDER, _wavelength, 1, 1024);
    addProcessPropertyInt("direction", "Direction","", IPL_INT_SLIDER, _direction, 0, 360);
    addProcessPropertyInt("decay", "Decay","", IPL_INT_SLIDER, _decay, 0, 1024);
}

void IPLSynthesize::destroy()
{
    delete _result;
}

bool IPLSynthesize::processInputData(IPLImage*, int, bool)
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
    _direction  = getProcessPropertyInt("direction");
    _decay      = getProcessPropertyInt("decay");

    _result = new IPLImage( IPLData::IMAGE_GRAYSCALE, _width, _height );

    double dx = (double)_width / 2.0;
    double dy = (double)_height / 2.0;
    double direction = _direction / 180.0 * PI; // deg to rad

    IPLImagePlane* plane = _result->plane( 0 );

    int progress = 0;
    int maxProgress = _result->height();

    switch( _type )
    {
    case 0: // plane wave
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
    case 1:// center wave
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
