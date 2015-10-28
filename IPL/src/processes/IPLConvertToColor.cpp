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

#include "IPLConvertToColor.h"

void IPLConvertToColor::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLConvertToColor");
    setTitle("Convert to Color");
    setKeywords("grayscale");
    setCategory(IPLProcess::CATEGORY_CONVERSIONS);

    // inputs and outputs
    addInput("Gray Image", IPL_IMAGE_GRAYSCALE);
    addOutput("Image", IPL_IMAGE_COLOR);
}

void IPLConvertToColor::destroy()
{
    delete _result;
}

bool IPLConvertToColor::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();

    int progress = 0;
    int maxProgress = image->height();

    _result = new IPLImage( IPL_IMAGE_COLOR, width, height );

    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);

        for(int x=0; x<width; x++)
        {
            ipl_basetype value = image->plane(0)->p(x, y);
            _result->plane(0)->p(x, y) = value;
            _result->plane(1)->p(x, y) = value;
            _result->plane(2)->p(x, y) = value;
        }
    }

    return true;
}

IPLImage* IPLConvertToColor::getResultData( int )
{
    return _result;
}
