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

#include "IPLStretchContrast.h"

void IPLStretchContrast::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLStretchContrast");
    setTitle("Stretch Contrast");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);
}

void IPLStretchContrast::destroy()
{
    delete _result;
}

bool IPLStretchContrast::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );

        // compute histogram
        int* seq = new int[256];
        for(int i=0; i<256; i++) seq[i] = 0;
        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                int index = 255 * plane->p(x,y);
                seq[index]++;
            }
        }

        // find limits
        int low = 0;
        while( !seq[low] ) low++;
        int high = 255;
        while( !seq[high] ) high--;

        float f_low  = low * FACTOR_TO_FLOAT;
        float f_high = high * FACTOR_TO_FLOAT;

        std::stringstream s;
        s << "Low: ";
        s << low;
        addInformation(s.str());

        std::stringstream s2;
        s2 << "High: ";
        s2 << high;
        addInformation(s2.str());

        if(high>low)
        {
            for(int y=0; y<height; y++)
            {
                // progress
                notifyProgressEventHandler(100*progress++/maxProgress);
                for(int x=0; x<width; x++)
                {
                    newplane->p(x,y) =  (plane->p(x,y) - f_low) / (f_high - f_low);
                }
            }
        }

        delete seq;
    }
    return true;
}

IPLData* IPLStretchContrast::getResultData( int )
{
    return _result;
}
