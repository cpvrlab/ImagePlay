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

#include "IPLFalseColor.h"

void IPLFalseColor::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLFalseColor");
    setTitle("False Color");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);
    setKeywords("heatmap");

    // inputs and outputs
    addInput("Grayscale Image", IPLData::IMAGE_GRAYSCALE);
    addOutput("Image", IPLImage::IMAGE_COLOR);
}

void IPLFalseColor::destroy()
{
    delete _result;
}

bool IPLFalseColor::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage(IPLImage::IMAGE_COLOR, width, height );

    int progress = 0;
    int maxProgress = image->height();

    IPLImagePlane* plane = image->plane(0);
    IPLImagePlane* r = _result->plane( 0 );
    IPLImagePlane* g = _result->plane( 1 );
    IPLImagePlane* b = _result->plane( 2 );
    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);
        for(int x=0; x<width; x++)
        {
            r->p(x,y) = 1.0;
            g->p(x,y) = 1.0;
            b->p(x,y) = 1.0;
            double v = plane->p(x,y);
            if ( v < 0.25 )
            {
                r->p(x,y) = 0.0;
                g->p(x,y) = 4*v;
                b->p(x,y) = 1.0;
            }
            else if ( v < 0.5 )
            {
                r->p(x,y) = 0.0;
                g->p(x,y) = 1.0;
                b->p(x,y) = (1.0-4*(v-0.25));
            }
            else if ( v < 0.75 )
            {
                r->p(x,y) = 4*(v-0.5);
                g->p(x,y) = 1.0;
                b->p(x,y) = 0.0;
            }
            else if ( v <= 1.0 )
            {
                r->p(x,y) = 1.0;
                g->p(x,y) = (1.0-4*(v-0.75));
                b->p(x,y) = 0.0;
            }
        }
    }
    return true;
}

IPLData* IPLFalseColor::getResultData( int )
{
    return _result;
}
