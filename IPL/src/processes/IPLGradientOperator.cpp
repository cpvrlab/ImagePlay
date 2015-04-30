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

#include "IPLGradientOperator.h"

void IPLGradientOperator::init()
{
    // init
    _result = NULL;

    // basic settings
    setClassName("IPLGradientOperator");
    setTitle("Gradient Operator");
    setCategory(IPLProcess::CATEGORY_GRADIENTS);
    setKeywords("Fast Gradient, Roberts, Sobel, Cubic Spline");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_GRAYSCALE);
    addOutput("Image", IPLData::IMAGE_ORIENTED);

    // properties
    addProcessPropertyInt("algorithm", "Algorithm:Fast Gradient|Roberts|Sobel|Cubic Spline", "", 0, IPL_WIDGET_RADIOBUTTONS);
}

void IPLGradientOperator::destroy()
{
    delete _result;
}

bool IPLGradientOperator::processInputData(IPLImage* image , int, bool)
{
    int width = image->width();
    int height = image->height();

    // delete previous result
    delete _result;
    _result = new IPLOrientedImage(width, height);

    // get properties
    //int algorithm = getProcessPropertyInt("algorithm");

    // fast gradient
    int progress = 0;
    int maxProgress = height*width;
    for(int x=1; x<width; x++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);
        for(int y=1; y<height; y++)
        {
            double Dx = image->plane(0)->p(x,y) - image->plane(0)->p(x-1, y);
            double Dy = image->plane(0)->p(x,y) - image->plane(0)->p(x, y-1);
            double magnitude = sqrt( Dx * Dx + Dy * Dy );
            double phase = (Dx!=0.0 || Dy!=0.0 )? atan2( -Dy, Dx ) + PI/2.0 : 0.0;

            while( phase > PI ) phase -= 2.0 * PI;
            while( phase < -PI ) phase += 2.0 * PI;

            // phase 0.0-1.0
            phase /= 2 * PI;

            _result->phase(x,y) = phase;
            _result->magnitude(x,y) = magnitude;
        }
    }

    return true;
}

IPLImage* IPLGradientOperator::getResultData( int )
{
    return _result;
}
