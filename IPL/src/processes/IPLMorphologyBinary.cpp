#include "IPLMorphologyBinary.h"

void IPLMorphologyBinary::init()
{
    // init
    _result = NULL;

    // basic settings
    setClassName("IPLMorphologyBinary");
    setTitle("Binary Morphology");
    setCategory(IPLProcess::CATEGORY_MORPHOLOGY);

    // default value
    // 0 0 0
    // 0 1 0
    // 0 0 0
    int nrElements = 9;
    _kernel.clear();
    for(int i=0; i<nrElements; i++)
    {
        _kernel.push_back((i==4 ? 1 : 0));
    }

    _operation = 0;
    _iterations = 1;

    // inputs and outputs
    addInput("Image", IPLImage::IMAGE_BW);
    addOutput("Image", IPLImage::IMAGE_BW);

    // properties
    addProcessPropertyVectorInt("kernel", "Kernel", "", _kernel, IPL_WIDGET_BINARY_MORPHOLOGY);
    addProcessPropertyInt("iterations", "Iterations",
                          "Run the algorithm x times\nCaution: big kernels and too many iterations can take a long time to compute!",
                          _iterations, IPL_WIDGET_SLIDER, 1, 16);
    addProcessPropertyInt("operation", "Operation:Dilate|Erode|Opening|Closing", "", _operation, IPL_WIDGET_RADIOBUTTONS);
}

void IPLMorphologyBinary::destroy()
{
    delete _result;
}

bool IPLMorphologyBinary::processInputData(IPLImage* image, int, bool)
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
    _iterations = getProcessPropertyInt("iterations");
    _operation  = getProcessPropertyInt("operation");
//    _propertyMutex.unlock();

    IPLImagePlane* inputPlane = image->plane( 0 );
    IPLImagePlane* resultPlane = _result->plane( 0 );

    IPLImagePlane* workingPlane = new IPLImagePlane(width, height);

    // the algorithm needs a working plane
    for(int x=0; x<width; x++)
    {
        for(int y=0; y<height; y++)
        {
            resultPlane->p(x,y) = inputPlane->p(x,y);
            workingPlane->p(x,y) = inputPlane->p(x,y);
        }
    }


    _progress = 0;
    _maxProgress = image->height() * image->getNumberOfPlanes() * _iterations;

    /// @todo implement manhattan distance threshold instead
    /// of stupid iterations...

    #pragma omp parallel for schedule(dynamic)
    for(int it=0; it<_iterations; it++)
    {
        // DILATE
        if(_operation == 0)
        {
            dilate(workingPlane, resultPlane);
        }
        // ERODE
        else if(_operation == 1)
        {
            if(it==0)
                _maxProgress *= 4;

            invert(workingPlane);
            invert(resultPlane);
            dilate(workingPlane, resultPlane);
            invert(resultPlane);
        }
        // OPENING (ERODE, DILATE)
        else if(_operation == 2)
        {
            if(it==0)
                _maxProgress *= 4;

            invert(workingPlane);
            IPLImagePlane* tmpPlane = new IPLImagePlane(width, height);
            // the algorithm needs a working plane
            for(int x=0; x<width; x++)
            {
                for(int y=0; y<height; y++)
                {
                    tmpPlane->p(x,y) = workingPlane->p(x,y);
                }
            }
            dilate(workingPlane, tmpPlane);
            invert(tmpPlane);
            // the algorithm needs a working plane
            for(int x=0; x<width; x++)
            {
                for(int y=0; y<height; y++)
                {
                    resultPlane->p(x,y) = tmpPlane->p(x,y);
                }
            }
            dilate(tmpPlane, resultPlane);
        }
        // CLOSING (DILATE, ERODE)
        else if(_operation == 3)
        {
            if(it==0)
                _maxProgress *= 4;

            IPLImagePlane* tmpPlane = new IPLImagePlane(width, height);
            dilate(workingPlane, tmpPlane);
            invert(tmpPlane);
            // the algorithm needs a working plane
            for(int x=0; x<width; x++)
            {
                for(int y=0; y<height; y++)
                {
                    resultPlane->p(x,y) = tmpPlane->p(x,y);
                }
            }
            dilate(tmpPlane, resultPlane);
            invert(resultPlane);
        }


        // copy resultPlane back to workingPlane if still iterating
        if(it < _iterations-1)
        {
            for(int y=0; y<height; y++)
            {
                for(int x=0; x<width; x++)
                {
                    workingPlane->p(x, y) = resultPlane->p(x, y);
                }
            }
        }
    }

    return true;
}

void IPLMorphologyBinary::invert(IPLImagePlane* inputPlane)
{
    int width = inputPlane->width();
    int height = inputPlane->height();

    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*_progress++/_maxProgress);

        for(int x=0; x<width; x++)
        {
            inputPlane->p(x,y) = 1.0f - inputPlane->p(x,y);
        }
    }
}

void IPLMorphologyBinary::dilate(IPLImagePlane* workingPlane, IPLImagePlane* resultPlane)
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
            if(workingPlane->p(x, y) == 1.0f)
            {
                int i = 0;
                for( int ky=-kernelOffset; ky<=kernelOffset; ky++ )
                {
                    for( int kx=-kernelOffset; kx<=kernelOffset; kx++ )
                    {
                        int kernelValue = _kernel[i++];
                        int r = x+kx;
                        int c = y+ky;
                        if(kernelValue == 1)
                        {
                            resultPlane->bp(r,c) = 1.0f;
                        }
                    }
                }
            }
        }
    }
}

IPLImage* IPLMorphologyBinary::getResultData( int )
{
    return _result;
}
