#include "IPLNegate.h"

void IPLNegate::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLNegate");
    setTitle("Negate Image");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);
    setKeywords("inverse, invert");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);
}

void IPLNegate::destroy()
{
    delete _result;
}

bool IPLNegate::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

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
                    newplane->p(x,y) = 1.0f - plane->p(x,y);
                }
            }
        }
    }
    return true;
}

IPLData* IPLNegate::getResultData( int )
{
    return _result;
}
