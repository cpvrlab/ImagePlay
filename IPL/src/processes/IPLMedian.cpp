#include "IPLMedian.h"

void IPLMedian::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLMedian");
    setTitle("Median Operator");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", IPL_INT_SLIDER_ODD, 3, 3, 9);
}

void IPLMedian::destroy()
{
    delete _result;
}

bool IPLMedian::processInputData(IPLImage* image , int, bool)
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

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );

        ipl_basetype* list = new ipl_basetype[area];

        for(int x=0; x<width; x++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int y=0; y<height; y++)
            {
                int i =0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        list[i++] = plane->bp(x+kx, y+ky);
                    }
                }

                // insert sort list
                for( int k=area; k>=0; k--)
                {
                    int j = k+1;
                    ipl_basetype temp = list[k];
                    while( j < area && temp > list[j] )
                    {
                        list[j-1] = list[j];
                        j++;
                    }
                    list[j-1] = temp;
                }
                newplane->p(x,y) = list[area/2];
            }
        }
    }
    return true;
}

IPLData* IPLMedian::getResultData( int )
{
    return _result;
}
