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

#include "IPLMarkImage.h"

void IPLMarkImage::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLMarkImage");
    setTitle("Mark Range");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);
    setDescription("Mark a level range in the image histogram with a selected color. The background"
                   "may be cleared or it may retain the original image. The resulting"
                   "image is a color image and has to be converted if used as a mask.");

    // inputs and outputs
    addInput("Grayscale Image", IPL_IMAGE_GRAYSCALE);
    addOutput("Image", IPL_IMAGE_COLOR);

    // properties
    addProcessPropertyDouble("min", "Min", "", 0.4, IPL_WIDGET_SLIDER, 0.0, 1.0);
    addProcessPropertyDouble("max", "Max", "", 0.6, IPL_WIDGET_SLIDER, 0.0, 1.0);
    addProcessPropertyInt("mode", "Mode:Show Background|Mask Only", "", 0, IPL_WIDGET_COMBOBOX);
    addProcessPropertyColor("color", "Color", "", IPLColor(1.0,0.0,0.0), IPL_WIDGET_COLOR_RGB);
    addProcessPropertyBool("invert", "Invert Mask", "", false, IPL_WIDGET_CHECKBOXES);
}

void IPLMarkImage::destroy()
{
    delete _result;
}

bool IPLMarkImage::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage(IPL_IMAGE_COLOR, width, height );

    // get properties
    float   min         = getProcessPropertyDouble("min");
    float   max         = getProcessPropertyDouble("max");
    IPLColor color      = getProcessPropertyColor("color");
    int     mode        = getProcessPropertyInt("mode");
    bool    invert      = getProcessPropertyBool("invert");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();

    IPLImagePlane* plane = image->plane(0);
    IPLImagePlane* r = _result->plane(0);
    IPLImagePlane* g = _result->plane(1);
    IPLImagePlane* b = _result->plane(2);

    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);
        for(int x=0; x<width; x++)
        {
            if(plane->p(x,y) >= min && plane->p(x,y) <= max)
            {
                r->p(x,y) = color.red();
                g->p(x,y) = color.green();
                b->p(x,y) = color.blue();
            }
            else
            {
                r->p(x,y) = mode ? 0.0 : plane->p(x,y);
                g->p(x,y) = mode ? 0.0 : plane->p(x,y);
                b->p(x,y) = mode ? 0.0 : plane->p(x,y);
            }
        }
    }
    return true;
}

IPLData* IPLMarkImage::getResultData( int )
{
    return _result;
}
