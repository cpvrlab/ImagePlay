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

#include "IPLFlipImage.h"

void IPLFlipImage::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLFlipImage");
    setTitle("Flip Image");
    setCategory(IPLProcess::CATEGORY_GEOMETRY);
    setDescription("Flip an image horizontally or vertically.");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);

    // properties
    addProcessPropertyInt("direction", "Direction:Horizontal|Vertical", "", 0, IPL_WIDGET_RADIOBUTTONS);
}

void IPLFlipImage::destroy()
{
    delete _result;
}

bool IPLFlipImage::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage(image->type(), width, height);

    // get properties
    int direction = getProcessPropertyInt("direction");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        notifyProgressEventHandler(100*progress++/maxProgress);

        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );
        for(int x=0; x < _result->width(); x++)
        {
            for(int y=0; y < _result->height(); y++)
             {
                if(direction == 0)
                {
                    // horizontal flip
                    int x2 = width - x - 1;
                    newplane->p(x,y) = plane->p(x2,y);
                }
                else
                {
                    // vertical flip
                    int y2 = height - y - 1;
                    newplane->p(x,y) = plane->p(x,y2);
                }

             }
        }
    }

    return true;
}

IPLImage* IPLFlipImage::getResultData( int )
{
    return _result;
}
