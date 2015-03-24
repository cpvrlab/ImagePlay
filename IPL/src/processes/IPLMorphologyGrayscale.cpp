#include "IPLMorphologyGrayscale.h"

void IPLMorphologyGrayscale::init()
{
    // init
    _result = NULL;

    // basic settings
    setClassName("IPLMorphologyGrayscale");
    setTitle("Grayscale Morphology");
    setCategory(IPLProcess::CATEGORY_MORPHOLOGY);

    // default value: 0
    int nrElements = 9;
    for(int i=0; i<nrElements; i++)
    {
        _kernel.push_back(0);
    }

    _operation = 0;
    _iterations = 1;

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLData::IMAGE_COLOR);

    // properties
//    _properties["kernel"]       = new IPLProcessPropertyVectorInt(this, 0, "Kernel", "", IPL_INT_GRAYSCALE_MORPHOLOGY, _kernel);
    addProcessPropertyVectorInt("kernel", "Kernel", "", IPL_INT_BINARY_MORPHOLOGY, _kernel);
    addProcessPropertyInt("operation", "Operation:Dilate|Erode|Open|Close", "", IPL_INT_RADIOBUTTONS, _operation);
}

void IPLMorphologyGrayscale::destroy()
{
    delete _result;
}

bool IPLMorphologyGrayscale::processInputData(IPLImage* image, int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();

    // copy constructor doesnt work:
    // _result = new IPLImage(*image);

    _result = new IPLImage( image->type(), width, height);

    // get properties
    _kernel     = getProcessPropertyVectorInt("kernel");
    //_iterations = ((IPLProcessPropertyInt*)               _properties["iterations"])->value();
    _operation  = getProcessPropertyInt("operation");

    #pragma omp parallel for
    for(int planeNr = 0; planeNr < image->getNumberOfPlanes(); planeNr++)
    {
        IPLImagePlane* inputPlane = image->plane( planeNr );
        IPLImagePlane* resultPlane = _result->plane( planeNr );

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

        /// @todo implement manhattan distance threshold instead
        /// of stupid iterations...
        for(int it=0; it<_iterations; it++)
        {
            // DILATE
            if(_operation == 0)
            {
                dilate(workingPlane, resultPlane);
            }
            // ERODE
            /*else if(_operation == 1)
            {
                invert(workingPlane);
                invert(resultPlane);
                dilate(workingPlane, resultPlane);
                invert(resultPlane);
            }
            // OPENING (ERODE, DILATE)
            else if(_operation == 2)
            {
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
            }*/
        }
    }

    return true;
}
/*
void IPLMorphologyGrayscale::dilate(IPLImagePlane* workingPlane, IPLImagePlane* resultPlane)
{
    int kernelOffset = (int)sqrt((float)_kernel.size()) / 2;

    int width = workingPlane->width();
    int height = workingPlane->height();

    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            std::vector<uchar> candidates;
            int i = 0;
            for( int ky=-kernelOffset; ky<=kernelOffset; ky++ )
            {
                for( int kx=-kernelOffset; kx<=kernelOffset; kx++ )
                {
                    int kernelValue = _kernel[i++];
                    int r = x+kx;
                    int c = y+ky;
                    if(kernelValue > -1)
                    {
                        candidates.push_back(workingPlane->bp(r,c) + kernelValue);
                    }
                }
            }

            // the result is the max of the candiate pixels
            uchar value = max_value(candidates);

            // clamp 0-255
            value = value < 0 ? 0 : (value > 255 ? 255 : value);
            resultPlane->bp(x,y) = value;
        }
    }
}*/

void IPLMorphologyGrayscale::dilate(IPLImagePlane* workingPlane, IPLImagePlane* resultPlane)
{
    int kernelOffset = (int)sqrt((float)_kernel.size()) / 2;

    int width = workingPlane->width();
    int height = workingPlane->height();

    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            ipl_basetype maxValue = 0.0f;
            int i = 0;
            for( int ky=-kernelOffset; ky<=kernelOffset; ky++ )
            {
                for( int kx=-kernelOffset; kx<=kernelOffset; kx++ )
                {
                    int kernelValue = _kernel[i++];
                    int r = x-kx;
                    int c = y-ky;
                    ipl_basetype value = workingPlane->bp(r,c);

                    if(kernelValue > 0)
                        maxValue = value > maxValue ? value : maxValue;
                }
            }

            resultPlane->bp(x,y) = maxValue;
        }
    }
}

uchar IPLMorphologyGrayscale::max_value(std::vector<uchar> a)
{
    uchar max = 0;
    for(size_t i=0; i<a.size(); i++)
    {
        max = std::max(max, a[i]);
    }
    return max;
}

IPLImage* IPLMorphologyGrayscale::getResultData( int )
{
    return _result;
}