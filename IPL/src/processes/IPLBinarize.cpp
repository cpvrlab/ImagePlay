#include "IPLBinarize.h"

void IPLBinarize::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLBinarize");
    setTitle("Binarize");
    setKeywords("threshold");
    setDescription("Converts an image with the color format binary, gray-scale, or color to a binary image. The threshold can be set between 0.0 and 1.0");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Gray Image", IPLImage::IMAGE_BW);

    // properties
    addProcessPropertyDouble("threshold", "Threshold", "0.0 < threshold < 1.0", 0.5, IPL_WIDGET_SLIDER, 0.0, 1.0);
}

void IPLBinarize::destroy()
{
    delete _result;
}

bool IPLBinarize::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    if( image->type() == IPLData::IMAGE_GRAYSCALE )
        _result = new IPLImage( IPLImage::IMAGE_BW, width, height );
    else
        _result = new IPLImage( image->type(), width, height );

    // get properties
    float threshold = getProcessPropertyDouble("threshold");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );
        for(int y=0; y<height; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);
            for(int x=0; x<width; x++)
            {
                newplane->p(x,y) = (plane->p(x,y) < threshold) ? 0.0f : 1.0f;
            }
        }
    }
    return true;
}

IPLImage* IPLBinarize::getResultData(int)
{
    return _result;
}
