#include "IPLBinarizeUnimodal.h"

void IPLBinarizeUnimodal::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLBinarizeUnimodal");
    setTitle("Binarize Unimodal");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);
    setDescription("Binarize image using threshold computed with Rosin’s unimodal method.");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
}

void IPLBinarizeUnimodal::destroy()
{
    delete _result;
}

bool IPLBinarizeUnimodal::processInputData(IPLImage* image , int, bool)
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

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );

        int p[256] = { 0 };

        for( int y = 0; y < height; ++y )
        {
            for( int x = 0; x < width; ++x )
            {
                int index = plane->p(x,y) * 255;
                p[index]++;
            }
        }

        // determine maxEntry
        int maxBin = 0;
        int maxFrequency = 0;
        for( int k=1; k<255; ++k )
        {
            if( p[k] > maxFrequency )
            {
                maxFrequency = p[k];
                maxBin = k;
            }
        }

        // determine zeroEntry
        int zeroBin = 0;
        int x0, x1;
        if( maxBin >= 128 )
        {
            zeroBin = 0;
            while( p[zeroBin]==0 ) ++zeroBin;
            x0 = zeroBin;
            x1 = maxBin;
        }
        else
        {
            zeroBin = 255;
            while( p[zeroBin]==0 ) --zeroBin;
            x0 = maxBin;
            x1 = zeroBin;
        }

        int y0 = p[x0];
        int y1 = p[x1];
        double a = y0 - y1;
        double b = x1 - x0;
        double c = x0*y1 - x1*y0;
        double d = sqrt( a*a + b*b );

        int T = 0;
        if( d != 0.0 )
        {
            double maxDist = 0.0;
            for( int k=x0; k<=x1; ++k )
            {
                double distance = abs( ( a*k + b*p[k] + c ) / d );
                if( distance > maxDist )
                {
                    maxDist = distance;
                    T = k;
                }
            }
        }

        ipl_basetype threshold = T * FACTOR_TO_FLOAT;

        std::stringstream s;
        s << "Automatic Threshold: ";
        s << threshold;
        addInformation(s.str());

        for(int y = 0; y < height; y++)
        {            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);
            for(int x = 0; x < width; x++)
            {
                newplane->p(x,y)= (plane->p(x,y) > threshold)? 1.0 : 0.0;
            }
        }
    }
    return true;
}

IPLData* IPLBinarizeUnimodal::getResultData( int )
{
    return _result;
}
