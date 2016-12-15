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
    _kernel.clear();
    for(int i=0; i<nrElements; i++)
    {
        _kernel.push_back((i==4 ? 1 : 0));
    }

    // inputs and outputs
    addInput("Image", IPL_IMAGE_BW);
    addOutput("Image", IPL_IMAGE_BW);

    // properties
    addProcessPropertyVectorInt("kernel", "Kernel", "", _kernel, IPL_WIDGET_BINARY_MORPHOLOGY_TRISTATE);
}

void IPLMorphologyHitMiss::destroy()
{
    delete _result;
}

bool IPLMorphologyHitMiss::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();

    // copy constructor doesnt work:
    // _result = new IPLImage(*image);

    _result = new IPLImage( IPL_IMAGE_BW, width, height);

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
                    if(i >= (int) _kernel.size())
                        continue;

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
