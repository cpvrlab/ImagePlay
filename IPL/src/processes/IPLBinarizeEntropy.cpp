#include "IPLBinarizeEntropy.h"

void IPLBinarizeEntropy::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLBinarizeEntropy");
    setTitle("Binarize Entropy");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);
    setDescription("Binarize image using threshold computed with maximum entropy method.");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);
}

void IPLBinarizeEntropy::destroy()
{
    delete _result;
}

bool IPLBinarizeEntropy::processInputData(IPLImage* image , int, bool)
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

        // prepare histogram
        double p[256];
        for( int i=0; i<256; i++ )
            p[i] = 0.0;
        for( int y=0; y < plane->height(); ++y )
        {
            for( int x=0; x < plane->width(); ++x )
            {
                int index = plane->p(x,y) * 255;
                p[index]++;
            }
        }

        for( int i=0; i<256; i++ )
        {
            p[i] /= plane->width()*plane->height();
        }

        // determine threshold
        double Hg = 0.0;
        for( int k=0; k<256; ++k )
            Hg += (p[k])? - p[k] *log( p[k] ) : 0.0;

        double maxEntropy = 0.0;
        double Pb = 0.0;
        double Ht = 0.0;
        float threshold = 0;
        for( int k=0; k<256; ++k )
        {
            Pb += p[k];
            Ht += (p[k])? - p[k] *log( p[k] ) : 0.0;

            double tmp = Pb * (1.0 - Pb);
            double entropy = ( tmp > 0.0 )? log( tmp ) + Ht / Pb + (Hg - Ht) / (1.0 - Pb) : 0.0;

            if( entropy > maxEntropy )
            {
                maxEntropy = entropy;
                threshold = k;
            }
        }

        // convert back to float
        threshold *= FACTOR_TO_FLOAT;

        std::stringstream s;
        s << "Automatic Threshold: ";
        s << threshold;
        addInformation(s.str());

        std::stringstream s2;
        s2 << "Maximum Entropy: ";
        s2 << maxEntropy;
        addInformation(s2.str());

        // apply
        for(int y=0; y<height; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);
            for(int x=0; x<width; x++)
            {
                newplane->p(x,y) = (plane->p(x,y) > threshold) ? 1.0f : 0.0f;
            }
        }
    }
    return true;
}

IPLData* IPLBinarizeEntropy::getResultData( int )
{
    return _result;
}
