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

#include "IPLRandomPoint.h"

void IPLRandomPoint::init()
{
    // init
    _image = NULL;
    _point = NULL;

    // basic settings
    setClassName("IPLRandomPoint");
    setTitle("Random Point");
    setCategory(IPLProcess::CATEGORY_OBJECTS);

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);
    addOutput("Random Point", IPL_POINT);

    // properties
    addProcessPropertyDouble("threshold", "Threshold", "0.0 < threshold < 1.0", 0.5, IPL_WIDGET_SLIDER, 0.0, 1.0);
}

void IPLRandomPoint::destroy()
{
    delete _image;
    delete _point;
}

bool IPLRandomPoint::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _image;
    _image = new IPLImage(*image);

    int width = image->width();
    int height = image->height();

    delete _point;
    _point = new IPLPoint;

    srand(time(0));
    int x = rand() % width;
    int y = rand() % height;

    _point->setX(x);
    _point->setY(y);

    return true;
}

IPLData* IPLRandomPoint::getResultData( int index )
{
    if(index == 0)
        return _image;
    else
        return _point;
}
