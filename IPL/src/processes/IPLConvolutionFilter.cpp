#include "IPLConvolutionFilter.h"

void IPLConvolutionFilter::init()
{
    // init
    _result     = NULL;
    _offset     = 0;
    _divisor    = 0;
    _borders    = 0;
    _kernel.clear();

    // basic settings
    setClassName("IPLConvolutionFilter");
    setTitle("2D Convolution");
    setKeywords("filter");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);
    setDescription("Convolution of a kernel with image.");
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_OPTIONAL);

    // default values
    // 0 0 0
    // 0 1 0
    // 0 0 0
    int nrElements = 9;
    for(int i=0; i<nrElements; i++)
    {
        // set the center to 1, all others to 0
        _kernel.push_back(i == nrElements/2 ? 1 : 0);
    }

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLData::IMAGE_COLOR);

    // properties
    addProcessPropertyVectorInt("kernel", "Kernel", "", IPL_INT_KERNEL, _kernel);
    addProcessPropertyBool("normalize", "Normalize", "Divisor is computed automatically", IPL_BOOL_CHECKBOX, true);
    addProcessPropertyInt("divisor", "Divisor", "", IPL_INT_SLIDER, 1, 1, 512);
    addProcessPropertyDouble("offset", "Offset", "", IPL_DOUBLE_SLIDER, 0.0, -1.0, 1.0);
    addProcessPropertyInt("borders", "Borders:Crop|Extend|Wrap", "Wrap is not available under OpenCV.", IPL_INT_RADIOBUTTONS, 0);
}

void IPLConvolutionFilter::destroy()
{
    delete _result;
}

bool IPLConvolutionFilter::processInputData(IPLImage* image , int, bool useOpenCV)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();

    // get properties
    _kernel     = getProcessPropertyVectorInt("kernel");
    _divisor    = getProcessPropertyInt("divisor");
    _offset     = getProcessPropertyDouble("offset");
    _normalize  = getProcessPropertyBool("normalize");
    _borders    = getProcessPropertyInt("borders");

    if(_normalize)
    {
        int sum = 0;
        for(size_t i=0; i<_kernel.size(); i++)
        {
            sum += _kernel[i];
        }
        _divisor = (sum != 0 ? sum : 1);
    }

    float divFactor = 1.0f/_divisor;

    int kernelWidth = (int)sqrt((float)_kernel.size());
    int kernelOffset = kernelWidth / 2;

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();

    if (!useOpenCV)
    {
        _result = new IPLImage( image->type(), width, height );
        #pragma omp parallel for default(shared)
        for( int planeNr=0; planeNr < image->getNumberOfPlanes(); planeNr++ )
        {
            IPLImagePlane* plane = image->plane( planeNr );
            IPLImagePlane* newplane = _result->plane( planeNr );

            for(int y=0; y<plane->height(); y++)
            {
                // progress
                notifyProgressEventHandler(100*progress++/maxProgress);

                for(int x=0; x<plane->width(); x++)
                {
                    float sum = 0;
                    int i = 0;
                    for( int ky=-kernelOffset; ky<=kernelOffset; ky++ )
                    {
                        for( int kx=-kernelOffset; kx<=kernelOffset; kx++ )
                        {
                            int h = _kernel[i++];
                            if( h )
                            {
                                if(_borders == 0)
                                {
                                    // Crop borders
                                    sum += plane->cp(x+kx, y+ky) * h;
                                }
                                else if(_borders == 1)
                                {
                                    // Extend borders
                                    sum += plane->bp(x+kx, y+ky) * h;
                                } else {
                                    // Wrap borders
                                    sum += plane->wp(x+kx, y+ky) * h;
                                }
                            }
                        }
                    }
                    sum = sum * divFactor + _offset;
                    sum = (sum>1.0) ? 1.0 : (sum<0) ? 0.0 : sum; // clamp to 0.0 - 1.0
                    newplane->p(x,y) = sum;
                }
            }
        }
    }
    else
    {
        cv::Mat src = image->toCvMat();
        cv::Mat dst;
        cv::Mat kernel(kernelWidth, kernelWidth, CV_32FC1 );

        int i = 0;
        for( int y=0; y < kernelWidth; y++ )
            for( int x=0; x < kernelWidth; x++ )
                kernel.at<float>(cv::Point(x,y)) = _kernel[i++];

        kernel *= divFactor;

        static const auto BORDER_TYPES = {
          cv::BORDER_CONSTANT,
          cv::BORDER_REPLICATE,
          cv::BORDER_WRAP
        };

        auto borderType = *(BORDER_TYPES.begin()+_borders);
        cv::filter2D(src, dst, -1, kernel, cv::Point(-1,-1), _offset, borderType);
        _result = new IPLImage(dst);
    }

    return true;
}

IPLImage* IPLConvolutionFilter::getResultData( int )
{
    return _result;
}
