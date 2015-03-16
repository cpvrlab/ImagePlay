#include "IPLMax.h"

void IPLMax::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLMax");
    setTitle("Max Operator");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", IPL_INT_SLIDER, 1,1,30);
}

void IPLMax::destroy()
{
    delete _result;
}

bool IPLMax::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

    // get properties
    int window = getProcessPropertyInt("window") * 2 - 1;

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    int w2 = window/2;

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );

        for(int x=0; x<width; x++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int y=0; y<height; y++)
            {
                ipl_basetype max = 0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        ipl_basetype img = plane->bp(x+kx, y+ky);
                        if( img > max ) max = img;
                    }
                }
                newplane->p(x,y) = max;
            }
        }
    }
    return true;
}

IPLData* IPLMax::getResultData( int )
{
    return _result;
}
