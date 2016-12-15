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

#include "IPLTriangleSegmentation.h"

void IPLTriangleSegmentation::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLTriangleSegmentation");
    setTitle("Triangle Segmentation");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);
}

void IPLTriangleSegmentation::destroy()
{
    delete _result;
}

bool IPLTriangleSegmentation::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    if( image->type() == IPL_IMAGE_GRAYSCALE )
        _result = new IPLImage( IPL_IMAGE_BW, width, height );
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

        int histogram[256];
        for( int i=0; i<256; i++ )
            histogram[i] = 0;

        // compute histogram
        for( int y=0; y<height; y++ )
        {
            for( int x=0; x<width; x++ )
            {
                int index = plane->p(x,y) * 255;
                histogram[index]++;
            }
        }

        int nrOfPixels = width * height;
        int minPixel = 0;
        while( !histogram[minPixel] ) minPixel++;
        int maxPixel = 255;
        while( !histogram[maxPixel] ) maxPixel--;
        double dI = (double)(maxPixel-minPixel)/(double)nrOfPixels;

        float threshold = -1;
        double maxDiff = 0.0;
        int pixelCount = 0;
        for( int i=minPixel; i<=maxPixel; i++ )
        {
            pixelCount += histogram[i];
            double diff = (double)pixelCount*dI - i + minPixel;
            if( diff > maxDiff )
            {
                maxDiff = diff;
                threshold = i;
            }
        }

        // convert back to float
        threshold *= FACTOR_TO_FLOAT;

        std::stringstream s;
        s << "Automatic Threshold: ";
        s << threshold;
        addInformation(s.str());

        std::stringstream s2;
        s2 << "Maximum Difference: ";
        s2 << maxDiff;
        addInformation(s2.str());

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

IPLData* IPLTriangleSegmentation::getResultData( int )
{
    return _result;
}
