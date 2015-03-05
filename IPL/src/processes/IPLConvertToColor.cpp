#include "IPLConvertToColor.h"

void IPLConvertToColor::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLConvertToGray");
    setTitle("Convert to Gray");
    setKeywords("grayscale");

    // inputs and outputs
    addInput("Gray Image", IPLData::IMAGE_GRAYSCALE);
    addOutput("Image", IPLData::IMAGE_COLOR);
}

void IPLConvertToColor::destroy()
{
    delete _result;
}

bool IPLConvertToColor::processInputData(IPLImage* image , int, bool useOpenCV)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();

    int progress = 0;
    int maxProgress = image->height();

    _result = new IPLImage( IPLData::IMAGE_COLOR, width, height );

    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);

        for(int x=0; x<width; x++)
        {
            ipl_basetype value = image->plane(0)->p(x, y);
            _result->plane(0)->p(x, y) = value;
            _result->plane(1)->p(x, y) = value;
            _result->plane(2)->p(x, y) = value;
        }
    }

    return true;
}

IPLImage* IPLConvertToColor::getResultData( int )
{
    return _result;
}
