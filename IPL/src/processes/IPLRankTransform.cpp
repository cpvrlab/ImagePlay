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

#include "IPLRankTransform.h"

void IPLRankTransform::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLRankTransform");
    setTitle("Rank Transform Operator");
    setCategory(IPLProcess::CATEGORY_MORPHOLOGY);

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", 3, IPL_WIDGET_SLIDER_ODD, 3, 9);
}

void IPLRankTransform::destroy()
{
    delete _result;
}

bool IPLRankTransform::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
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

    if(area<2)
    {
        addError("Window size too small.");
        return false;
    }

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
                int u = 0;
                int v = 0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        ipl_basetype img0 = plane->bp(x+kx, y+ky);
                        if (img < img0) u++;
                        if (img == img0) v++;
                    }
                }
                ipl_basetype r = u + v/2;
                r = area-r;
                ipl_basetype value = (r-1)/(area-1);
                value = value > 1.0 ? 1.0 : value;
                value = value < 0.0 ? 0.0 : value;
                newplane->p(x,y) = value;
            }
        }
    }
    return true;
}

IPLData* IPLRankTransform::getResultData( int )
{
    return _result;
}
