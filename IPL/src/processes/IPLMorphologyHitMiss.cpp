#include "IPLMorphologyHitMiss.h"

void IPLMorphologyHitMiss::init()
{
    // init
    _result = NULL;

    // basic settings
    setClassName("IPLMorphologyHitMiss");
    setTitle("Hit-Miss Morphology");
    setCategory(IPLProcess::CATEGORY_MORPHOLOGY);

    // default value
    // 0 0 0
    // 0 1 0
    // 0 0 0
    int nrElements = 9;
    for(int i=0; i<nrElements; i++)
    {
        _kernel.push_back((i==4 ? 1 : 0));
    }

    // inputs and outputs
    addInput("Image", IPLImage::IMAGE_BW);
    addOutput("Image", IPLImage::IMAGE_BW);

    // properties
    addProcessPropertyVectorInt("kernel", "Kernel", "", IPL_INT_BINARY_MORPHOLOGY_TRISTATE, _kernel);
}

void IPLMorphologyHitMiss::destroy()
{
    delete _result;
}

bool IPLMorphologyHitMiss::processInputData(IPLImage* image, int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();

    // copy constructor doesnt work:
    // _result = new IPLImage(*image);

    _result = new IPLImage( IPLImage::IMAGE_BW, width, height);

    // get properties
//    _propertyMutex.lock();
    _kernel     = getProcessPropertyVectorInt("kernel");
//    _propertyMutex.unlock();

    IPLImagePlane* inputPlane = image->plane( 0 );
    IPLImagePlane* resultPlane = _result->plane( 0 );

    IPLImagePlane* workingPlane = new IPLImagePlane(width, height);

    // the algorithm needs a working plane
    for(int x=0; x<width; x++)
    {
        for(int y=0; y<height; y++)
        {
            workingPlane->p(x,y) = inputPlane->p(x,y);
        }
    }


    _progress = 0;
    _maxProgress = image->height() * image->getNumberOfPlanes();

    hitmiss(workingPlane, resultPlane);

    return true;
}

void IPLMorphologyHitMiss::hitmiss(IPLImagePlane* workingPlane, IPLImagePlane* resultPlane)
{
    int kernelOffset = (int)sqrt((float)_kernel.size()) / 2;

    int width = workingPlane->width();
    int height = workingPlane->height();

    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*_progress++/_maxProgress);

        for(int x=0; x<width; x++)
        {
            int i = 0;
            bool success = true;
            for( int ky=-kernelOffset; ky<=kernelOffset && success; ky++ )
            {
                for( int kx=-kernelOffset; kx<=kernelOffset && success; kx++ )
                {
                    int kernelValue = _kernel[i++];
                    int r = x+kx;
                    int c = y+ky;
                    if(kernelValue > -1)
                        if(kernelValue != (workingPlane->bp(r,c) ? 1 : 0))
                            success = false; // breaks both for loops
                }
            }

            if(success)
                resultPlane->p(x,y) = 1.0f;
        }
    }
}

IPLImage* IPLMorphologyHitMiss::getResultData( int )
{
    return _result;
}
