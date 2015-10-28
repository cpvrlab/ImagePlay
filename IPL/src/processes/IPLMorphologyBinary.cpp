//#############################################################################
//
//  This file is part of ImagePlay.
//
//  ImagePlay is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ImagePlay is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ImagePlay.  If not, see <http://www.gnu.org/licenses/>.
//
//#############################################################################

#include "IPLMorphologyBinary.h"

#include <atomic>
#include <numeric>

void IPLMorphologyBinary::init()
{
    // init
    _result = NULL;

    // basic settings
    setClassName("IPLMorphologyBinary");
    setTitle("Binary Morphology");
    setCategory(IPLProcess::CATEGORY_MORPHOLOGY);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_OPTIONAL);

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
    addInput("Image", IPL_IMAGE_BW);
    addOutput("Image", IPL_IMAGE_BW);

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

// Both low-level morphology operators can be reduced to logic OR  or AND operations.
// Those, in turn, can be viewed as simple loops with an exit condition:
// We loop over all cells in the kernel and check for a predicate. If the predicate
// is not statisfied, we can exit the loop and color the center pixel accordingly.

// Example: Dilation: If all of the pixels corresponding to an active kernel cell are
//          zero, the center pixel will result in a zero value. Otherwise, the center
//          pixel is set to one. Therefore: T = 1, F = 0

//          For the erosion, the equation behaves exactly the opposite way
//          (i.e. T = 0, F = 1).

template<int T, int F, class CB>
void applyMorphology(IPLImagePlane &src, IPLImagePlane &dst, int iterations, const std::vector<bool> &kernel, CB progressCallback)
{
    int kernelOffset = (int)sqrt((float)kernel.size()) / 2;

    for (int i = 0; i < iterations; ++i)
    {
        #pragma omp parallel for default(shared)
        for (int y = 0; y < src.height(); ++y)
        {
            for (int x = 0; x < src.width(); ++x)
            {
                //TODO: Speed up this routine
                //There would be several possibilities such as usage of SIMD techniques
                //or the reduction of the source image (e.g. to unsigned char)

                bool cancel = false;
                auto &pixelValue = dst.p(x,y);
                int i = 0;
                for( int ky=-kernelOffset; !cancel && ky<=kernelOffset; ky++ )
                {
                    for( int kx=-kernelOffset; !cancel && kx<=kernelOffset; kx++ )
                    {
                        if (   x+kx < 0 || x+kx >= src.width()
                            || y+ky < 0 || y+ky >= src.height())
                            continue;

                        auto &p = src.p(x+kx,y+ky);
                        bool mask = kernel[i++];
                        bool pixel = p == (float)T;
                        cancel = mask && pixel;
                    }
                }

                pixelValue = cancel? T : F;
            }
            progressCallback();
        }
        std::swap(src,dst);
    }
    std::swap(src,dst);
}

template<class CB>
void dilate(const IPLImagePlane &src, IPLImagePlane &dst, int iterations, const std::vector<bool> &kernel, CB progressCallback)
{
    IPLImagePlane input(src); //Don't mutate the original image plane
    applyMorphology<1,0>(input,dst,iterations,kernel,progressCallback);
}

template<class CB>
void erode(const IPLImagePlane &src, IPLImagePlane &dst, int iterations, const std::vector<bool> &kernel, CB progressCallback)
{
    IPLImagePlane input(src); //Don't mutate the original image plane
    applyMorphology<0,1>(input,dst,iterations,kernel,progressCallback);
}

template<class CB>
void open(const IPLImagePlane &src, IPLImagePlane &dst, int iterations, const std::vector<bool> &kernel, CB progressCallback)
{
    IPLImagePlane input(src); //Don't mutate the original image plane
    erode (input,dst,iterations,kernel,progressCallback);
    std::swap(input,dst);
    dilate(input,dst,iterations,kernel,progressCallback);
}

template<class CB>
void close(const IPLImagePlane &src, IPLImagePlane &dst, int iterations, const std::vector<bool> &kernel, CB progressCallback)
{
    IPLImagePlane input(src); //Don't mutate the original image plane
    dilate(input,dst,iterations,kernel,progressCallback);
    std::swap(input,dst);
    erode (input,dst,iterations,kernel,progressCallback);
}


bool IPLMorphologyBinary::processInputData(IPLImage* image, int, bool useOpenCV)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();

    // get properties
    _kernel     = getProcessPropertyVectorInt("kernel");
    _iterations = getProcessPropertyInt("iterations");
    _operation  = getProcessPropertyInt("operation");

    if (std::accumulate(_kernel.begin(),_kernel.end(),0) == 0)
    {
        //Add an empty image - The image viewer currently may trigger
        //a segfault if we return NULL.
        _result = new IPLImage( IPL_IMAGE_BW, width, height);
        addError("Empty kernel.");
        return false;
    }

    // TODO: implement manhattan distance threshold instead of stupid iterations...
    // TODO: implement border interpolation properties

    enum Operation
    {
        DILATE = 0,
        ERODE,
        OPEN,
        CLOSE
    };

    if (!useOpenCV)
    {
        _result = new IPLImage( IPL_IMAGE_BW, width, height);

        //std::vector<bool> packs its elements bitwise into a vector of
        //bytes. The kernel therefore uses much less cpu cache this way.
        std::vector<bool> kernel;
        kernel.reserve(_kernel.size());
        for (auto &i: _kernel) kernel.push_back(i > 0);

        std::atomic<int> progress(0);
        int totalLines = image->height()*_iterations;
        auto updateProgress = [&]() {
            notifyProgressEventHandler(100*((float)++progress)/totalLines);
        };

        switch(_operation)
        {
        case DILATE:
            dilate(*image->plane(0),*_result->plane(0),_iterations,kernel,updateProgress);
            break;
        case ERODE:
            erode (*image->plane(0),*_result->plane(0),_iterations,kernel,updateProgress);
            break;
        case OPEN:
            totalLines *= 2;
            open  (*image->plane(0),*_result->plane(0),_iterations,kernel,updateProgress);
            break;
        case CLOSE:
            totalLines *= 2;
            close (*image->plane(0),*_result->plane(0),_iterations,kernel,updateProgress);
            break;
        }
    }
    else
    {
        notifyProgressEventHandler(-1);

        int kernelSize = (int)sqrt((float)_kernel.size());
        cv::Mat src = image->toCvMat();
        cv::Mat dst;

        cv::Mat kernel(kernelSize,kernelSize,CV_8UC1);
        int i = 0;
        for (auto pixel: _kernel)
            kernel.at<unsigned char>(i++) = pixel;

        static const int OPERATIONS[4] = {
            cv::MORPH_DILATE,
            cv::MORPH_ERODE,
            cv::MORPH_OPEN,
            cv::MORPH_CLOSE
        };

        cv::morphologyEx(src,dst,OPERATIONS[_operation],kernel,cv::Point(-1,-1),_iterations);
        _result = new IPLImage(dst);
    }

    return true;
}

IPLImage* IPLMorphologyBinary::getResultData( int )
{
    return _result;
}
