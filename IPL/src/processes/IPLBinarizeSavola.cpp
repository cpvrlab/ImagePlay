#include "IPLBinarizeSavola.h"

void IPLBinarizeSavola::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLBinarizeSavola");
    setTitle("Local Threshold (Savola)");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLData::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window Size", "",IPL_INT_SLIDER, 3, 3, 9);
    addProcessPropertyDouble("aboveMean", "Above Mean",
                             "Local Threshold as proposed by Savola et. al.",
                             IPL_DOUBLE_SLIDER, 0.0, 0.0, 1.0);
}

void IPLBinarizeSavola::destroy()
{
    delete _result;
}

bool IPLBinarizeSavola::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width  = image->width();
    int height = image->height();

    _result = new IPLImage(image->type(), width, height);

    // get properties
    int window = getProcessPropertyInt("window");
    double aboveMean = getProcessPropertyDouble("aboveMean");


    IPLImage* mean = new IPLImage(image->type(), width, height);
    IPLImage* deviation = new IPLImage(image->type(), width, height);

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );

        int w2 = window/2;
        float area = (float)(w2*2)*(float)(w2*2);
        float minI = FLT_MAX;
        float maxDeviation = 0.0;

        for(int y=0; y<height; y++)
        {
            // progress
           notifyProgressEventHandler(100*progress++/maxProgress);

            for(int x=0; x<width; x++)
            {
                if( plane->p(x,y) < minI ) minI = plane->p(x,y);
                float localMean = 0.0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        localMean += (float)plane->cp(x+kx,y+ky);
                    }
                }
                localMean /= area;
                mean->plane(planeNr)->p(x,y) = localMean;

                float dev = 0.0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        float diff =  (float)plane->cp(x+kx, y+ky) - localMean;
                        dev += diff * diff;
                    }
                }
                dev = sqrt( dev / area );
                deviation->plane(planeNr)->p(x,y) = dev;
                if( dev > maxDeviation ) maxDeviation = dev;
            }

            for(int x=w2; x<width-w2; x++)
            {
                for(int y=w2; y<height-w2; y++)
                {
                    float alpha = 1.0 - deviation->plane(planeNr)->p(x,y) / maxDeviation;
                    int T = (int) ( mean->plane(planeNr)->p(x,y) - aboveMean * alpha *( mean->plane(planeNr)->p(x,y) - minI ) );
                    newplane->p(x,y) = ( plane->p(x,y) >= T ) ? 0.0 : 1.0;
                }
            }
        }
    }

    return true;
}

IPLData* IPLBinarizeSavola::getResultData(int)
{
    return _result;
}
