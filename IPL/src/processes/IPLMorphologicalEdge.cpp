#include "IPLMorphologicalEdge.h"

void IPLMorphologicalEdge::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLMorphologicalEdge");
    setTitle("Morphological Edge Detector");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", IPL_INT_SLIDER, 1,1,30);
}

void IPLMorphologicalEdge::destroy()
{
    delete _result;
}

bool IPLMorphologicalEdge::processInputData(IPLImage* image , int, bool useOpenCV)
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
    int maxProgress = image->height() * image->getNumberOfPlanes() * 2;
    int nrOfPlanes = image->getNumberOfPlanes();

    int w2 = window/2;
    int area = window*window;

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );
        IPLImagePlane* average = new IPLImagePlane(width, height);

        for(int x=0; x<width; x++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);
            for(int y=0; y<height; y++)
            {
                ipl_basetype sum = 0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        if( kx || ky ) sum += plane->bp(x+kx, y+ky);
                    }
                }
                average->p(x,y) = sum;
            }
        }
        for(int x=0; x<width; x++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int y=0; y<height; y++)
            {
                int minc = (area-1);
                int maxc = 0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        ipl_basetype img = average->bp(x+kx, y+ky);
                        if( img > maxc) maxc = img;
                        if( img < minc) minc = img;
                    }
                }
                ipl_basetype img = average->p(x,y);
                ipl_basetype d1 = img - minc;
                ipl_basetype d2 = maxc - img;
                ipl_basetype min = (d1 < d2)? d1 : d2;
                min = (min<1.0)? min : 1.0;
                min = (min>0.0)? min : 0.0;
                newplane->p(x,y) = min;
            }
        }
        delete average;
    }
    return true;
}

IPLData* IPLMorphologicalEdge::getResultData( int )
{
    return _result;
}
