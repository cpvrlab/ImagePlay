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
    addInput("Complex Image", IPLData::IMAGE_COMPLEX);
    addOutput("Complex Image", IPLData::IMAGE_COMPLEX);
    //addOutput("Filter", IPLImage::IMAGE_GRAYSCALE);

    // properties
    addProcessPropertyInt("maskType", "Mask Type:Low Pass|High Pass|Band Pass|Band Stop", "", 0, IPL_WIDGET_RADIOBUTTONS);
    addProcessPropertyDouble("lowCutoff", "Low Cutoff", "", 0.0, IPL_WIDGET_SLIDER, 0.0, 1.0);
    addProcessPropertyDouble("highCutoff", "High Cutoff", "", 0.0, IPL_WIDGET_SLIDER, 0.0, 1.0);
    addProcessPropertyBool("keepDC", "Keep DC", "", false, IPL_WIDGET_CHECKBOXES);
}

void IPLFrequencyFilter::destroy()
{
    delete _result;
}

bool IPLFrequencyFilter::processInputData(IPLImage* data , int, bool)
{
    _input = new IPLComplexImage(*data->toComplexImage());

    // delete previous result
    delete _result;
    delete _filter;

    _result = new IPLComplexImage(*_input);
    int width = _result->width();
    int height = _result->height();
    _filter = new IPLImage(IPLImage::IMAGE_GRAYSCALE, width, height);


    // get properties
    int maskType = getProcessPropertyInt("maskType");
    double lowCutoff = getProcessPropertyDouble("lowCutoff");
    double highCutoff = getProcessPropertyDouble("highCutoff");
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
                case 1: if( range < lowRange )
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

IPLData* IPLFrequencyFilter::getResultData(int index)
{
    return _result;
}
