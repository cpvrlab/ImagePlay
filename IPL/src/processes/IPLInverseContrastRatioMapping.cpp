#include "IPLInverseContrastRatioMapping.h"

void IPLInverseContrastRatioMapping::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLInverseContrastRatioMapping");
    setTitle("Inverse Contrast Ratio Mapping");
    setKeywords("ICRM");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", IPL_INT_SLIDER_ODD, 3, 3, 15);
}

void IPLInverseContrastRatioMapping::destroy()
{
    delete _result;
}

bool IPLInverseContrastRatioMapping::processInputData(IPLImage* image , int, bool)
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
    double m1 = 0.0;
    double m2 = 0;

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
                double u = 0;
                double s = 0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        double img = plane->bp(x+kx, y+ky);
                        u += img;
                        s += (img * img);
                    }
                }
                img = (double)plane->bp(x,y);
                m1 += img;
                u /= (double)area;
                s = s/area - u*u;
                double gg = img / (1.0 + sqrt(s));

                if(gg > 1.0) gg = 1.0;
                m2 += gg;
                newplane->p(x,y) = gg;
            }
        }
        for( int y=0; y<height; y++ )
        {
            for( int x=0; x<width; x++ )
            {
                double temp = (m1 * newplane->bp(x,y) / m2);
                newplane->p(x,y) = ((temp > 1.0)? 1.0 : temp);
            }
        }
    }
    return true;
}

IPLData* IPLInverseContrastRatioMapping::getResultData( int )
{
    return _result;
}
