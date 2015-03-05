#include "IPLEqualizeHistogram.h"

void IPLEqualizeHistogram::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLEqualizeHistogram");
    setTitle("Equalize Histogram");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Gray Image", IPLImage::IMAGE_BW);

    // properties
    addProcessPropertyDouble("factor", "Factor", "", IPL_DOUBLE_SLIDER, 1.0, 0.0, 5.0);
}

void IPLEqualizeHistogram::destroy()
{
    delete _result;
}

bool IPLEqualizeHistogram::processInputData(IPLImage* image , int, bool useOpenCV)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

    // get properties
    float factor = getProcessPropertyDouble("factor");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );

        double equ[256];
        for( int i=0; i<256; i++ )equ[i] = 0.0;

        // compute histogram
        for( int x=0; x<width; x++ )
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);
            for( int y=0; y<height; y++ )
            {
                int index = plane->p(x,y) * 255;
                equ[index] += 1.0;
            }
        }

        double area = (double)width * (double)height;
        area *= area;
        double f = fabs( factor );

        for( int i=0; i<256; i++ )
            equ[i] = pow( equ[i]/area, f);

        for( int i=1; i<256; i++ )
            equ[i] += equ[i-1];

        for( int i=0; i<256; i++ )
            equ[i] *= 255.0 / equ[ 255 ];

        // equalize
        for(int y=0; y<height; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);
            for(int x=0; x<width; x++)
            {
                int index =  plane->p(x,y) * 255;
                newplane->p(x,y) = equ[index] / 255;
            }
        }
    }
    return true;
}

IPLData* IPLEqualizeHistogram::getResultData( int )
{
    return _result;
}
