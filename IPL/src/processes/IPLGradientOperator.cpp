#include "IPLGradientOperator.h"

void IPLGradientOperator::init()
{
    // init
    _result = NULL;

    // basic settings
    setClassName("IPLGradientOperator");
    setTitle("Gradient Operator");
    setCategory(IPLProcess::CATEGORY_GRADIENTS);
    setKeywords("Fast Gradient, Roberts, Sobel, Cubic Spline");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_GRAYSCALE);
    addOutput("Image", IPLData::IMAGE_ORIENTED);

    // properties
    addProcessPropertyInt("algorithm", "Algorithm:Fast Gradient|Roberts|Sobel|Cubic Spline", "", IPL_INT_RADIOBUTTONS, 0);
}

void IPLGradientOperator::destroy()
{
    delete _result;
}

bool IPLGradientOperator::processInputData(IPLImage* image , int, bool)
{
    int width = image->width();
    int height = image->height();

    // delete previous result
    delete _result;
    _result = new IPLOrientedImage(width, height);

    // get properties
    //int algorithm = getProcessPropertyInt("algorithm");

    // fast gradient
    int progress = 0;
    int maxProgress = height*width;
    for(int x=1; x<width; x++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);
        for(int y=1; y<height; y++)
        {
            double Dx = image->plane(0)->p(x,y) - image->plane(0)->p(x-1, y);
            double Dy = image->plane(0)->p(x,y) - image->plane(0)->p(x, y-1);
            double magnitude = sqrt( Dx * Dx + Dy * Dy );
            double phase = (Dx!=0.0 || Dy!=0.0 )? atan2( -Dy, Dx ) + PI/2.0 : 0.0;

            while( phase > PI ) phase -= 2.0 * PI;
            while( phase < -PI ) phase += 2.0 * PI;

            // phase 0.0-1.0
            phase /= 2 * PI;

            _result->phase(x,y) = phase;
            _result->magnitude(x,y) = magnitude;
        }
    }

    return true;
}

IPLImage* IPLGradientOperator::getResultData( int )
{
    return _result;
}
