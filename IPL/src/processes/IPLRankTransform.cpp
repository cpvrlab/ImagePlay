#include "IPLRankTransform.h"

void IPLRankTransform::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLRankTransform");
    setTitle("Rank Transform Operator");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", IPL_INT_SLIDER, 2, 2,30);
}

void IPLRankTransform::destroy()
{
    delete _result;
}

bool IPLRankTransform::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

    // get properties
    int window = getProcessPropertyInt("window");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    int w2 = window/2;
    int area = window*window;

    if(area<2)
    {
        addError("Window size too small.");
        return false;
    }

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
                ipl_basetype img = plane->p(x,y);
                int u = 0;
                int v = 0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        ipl_basetype img0 = plane->bp(x+kx, y+ky);
                        if (img < img0) u++;
                        if (img == img0) v++;
                    }
                }
                ipl_basetype r = u + v/2;
                r = area-r;
                ipl_basetype value = (r-1)/(area-1);
                value = value > 1.0 ? 1.0 : value;
                value = value < 0.0 ? 0.0 : value;
                newplane->p(x,y) = value;
            }
        }
    }
    return true;
}

IPLData* IPLRankTransform::getResultData( int )
{
    return _result;
}
