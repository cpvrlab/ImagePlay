#include "IPLBinarizeOtsu.h"

void IPLBinarizeOtsu::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLBinarizeOtsu");
    setTitle("Binarize Otsu");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);
    setDescription("Binarize image using threshold computed with Otsu’s method.");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Gray Image", IPLImage::IMAGE_BW);

    // properties
}

void IPLBinarizeOtsu::destroy()
{
    delete _result;
}

bool IPLBinarizeOtsu::processInputData(IPLImage* image , int, bool)
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




        double histogram[256];
        for(int i = 0; i < 256; i++)
            histogram[i] = 0.0;
        for(int y=0; y < height; y++)
        {
            for(int x=0; x < width; x++)
            {
                int index = plane->p(x,y) * 255;
                histogram[index]++;
            }
        }
        for( int i=0; i<256; i++ )
            histogram[i] /= width * height;

        // determine threshold
        double totalMean = 0.0;
        for( int k=0; k<256; ++k )
            totalMean += k * histogram[k];

        double maxVariance = 0.0;
        double zerothCumuMoment = 0.0;
        double firstCumuMoment = 0.0;
        int T = 0;  // threshold
        for( int k=0; k<256; ++k )
        {
            zerothCumuMoment += histogram[k];
            firstCumuMoment += k * histogram[k];

            double variance = (totalMean * zerothCumuMoment - firstCumuMoment);  // numerator
            variance *= variance;  // squared
            double denom = zerothCumuMoment * (1 - zerothCumuMoment);
            if( denom != 0.0 )
                variance /= denom;  // denominator

            if( variance > maxVariance )
            {
                maxVariance = variance;
                T = k;
            }
        }

        ipl_basetype threshold = T * FACTOR_TO_FLOAT;

        std::stringstream s;
        s << "Automatic Threshold: ";
        s << threshold;
        addInformation(s.str());

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

IPLData* IPLBinarizeOtsu::getResultData( int )
{
    return _result;
}
