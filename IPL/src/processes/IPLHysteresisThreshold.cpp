#include "IPLHysteresisThreshold.h"

void IPLHysteresisThreshold::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLHysteresisThreshold");
    setTitle("Hysteresis Threshold");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);
    setDescription("Apply hysteresis threshold.");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyDouble("lowThreshold", "Low Threshold", "", IPL_DOUBLE_SLIDER, 0.3, 0.0, 1.0);
    addProcessPropertyDouble("highThreshold", "High Threshold", "", IPL_DOUBLE_SLIDER, 0.6, 0.0, 1.0);
}

void IPLHysteresisThreshold::destroy()
{
    delete _result;
}

bool IPLHysteresisThreshold::processInputData(IPLImage* image , int, bool useOpenCV)
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
        {
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
    }
    return true;
}

IPLData* IPLHysteresisThreshold::getResultData( int )
{
    return _result;
}
