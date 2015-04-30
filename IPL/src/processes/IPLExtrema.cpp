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

#include "IPLExtrema.h"

void IPLExtrema::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLExtrema");
    setTitle("Extrema");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", 3, IPL_WIDGET_SLIDER, 3, 15);
}

void IPLExtrema::destroy()
{
    delete _result;
}

bool IPLExtrema::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

    // get properties
    float window = getProcessPropertyInt("window");

    int progress = 0;
    int maxProgress = image->width() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    int w2 = window/2;
    int area =window * window;

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );

        for(int x=w2; x<width-w2; x++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int y=w2; y<height-w2; y++)
            {
                ipl_basetype fmin = 1.0;
                ipl_basetype fmax = 0.0;
                ipl_basetype average  = 0;
                ipl_basetype img0 = plane->p( x, y );
                for( int ky=-w2; ky <= w2; ky++ )
                {
                    for( int kx=-w2; kx <= w2; kx++ )
                    {
                        ipl_basetype img = plane->p(x+kx, y+ky);

                        if(fmax < img) fmax = img;
                        if(fmin > img) fmin = img;
                        average += img;
                    }
                }
                average = average/area;
                ipl_basetype res = ( (img0 - fmin) > (fmax -img0) ) ? fmax : fmin;
                if( abs( average - img0 ) <= abs( res -img0 ) ) res = average;
                newplane->p(x,y) = res;
            }
        }
    }
    return true;
}

IPLData* IPLExtrema::getResultData( int )
{
    return _result;
}
