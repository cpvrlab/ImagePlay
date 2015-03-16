#include "IPLBinarizeKMeans.h"

void IPLBinarizeKMeans::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLBinarizeKMeans");
    setTitle("Binarize k-Means");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Gray Image", IPLImage::IMAGE_BW);
}

void IPLBinarizeKMeans::destroy()
{
    delete _result;
}

bool IPLBinarizeKMeans::processInputData(IPLImage* image , int, bool)
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

    //#pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );
        double histogram[256];
        for( int i=0; i<256; i++ )
            histogram[i] = 0.0;
        for( int y = 0; y < height; y++ )
        {
            for( int x = 0; x < width; x++ )
            {
                int index = plane->p(x,y) * 255;
                histogram[index]++;
            }
        }
        double mean = 0.0;
        for( int i=0; i<256; i++ )
            mean += i*histogram[i];

        mean /= width * height;

        double borderMean =  ( plane->p(0,0) +plane->p(width-1,0)
                             + plane->p(0,height-1) + plane->p(width-1,height-1) ) / 4.0;


        int T = (int) ( (mean + borderMean)/2.0 + 0.5 );

        bool changed = true;
        do
        {
            // compute means
            double meanB = 0.0;
            int count = 0;
            for( int k=0; k<T; k++ )
            {
                meanB += k * histogram[k];
                count += histogram[k];
            }
            if( count ) meanB /= count;

            double meanO = 0.0;
            count = 0;
            for( int k=T; k<256; k++ )
            {
                meanO += k * histogram[k];
                count += histogram[k];
            }
            if( count ) meanO /= count;

            int newT = (int)( ( meanB + meanO) / 2.0 + 0.5 );
            if( newT != T )
            {
                T = newT;
                changed = true;
            }
            else
                changed = false;
        } while( changed );

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

IPLData* IPLBinarizeKMeans::getResultData( int )
{
    return _result;
}
