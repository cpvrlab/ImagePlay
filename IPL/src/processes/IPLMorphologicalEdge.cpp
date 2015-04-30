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

#include "IPLMorphologicalEdge.h"

void IPLMorphologicalEdge::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLMorphologicalEdge");
    setTitle("Morphological Edge Detector");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", 3, IPL_WIDGET_SLIDER_ODD, 3, 9);
}

void IPLMorphologicalEdge::destroy()
{
    delete _result;
}

bool IPLMorphologicalEdge::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

    // get properties
    int window = getProcessPropertyInt("window");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes() * 2;
    int nrOfPlanes = image->getNumberOfPlanes();

    int w2 = window/2;
    int area = window*window;

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );
        IPLImagePlane* average = new IPLImagePlane(width, height);

        for(int x=w2; x<width-w2; x++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);
            for(int y=w2; y<height-w2; y++)
            {
                ipl_basetype sum = 0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        if( kx || ky ) sum += plane->p(x+kx, y+ky);
                    }
                }
                average->p(x,y) = sum;
            }
        }
        for(int x=w2; x<width-w2; x++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int y=w2; y<height-w2; y++)
            {
                float minc = (area-1);
                float maxc = 0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        ipl_basetype img = average->bp(x+kx, y+ky);
                        if( img > maxc) maxc = img;
                        if( img < minc) minc = img;
                    }
                }
                ipl_basetype img = average->p(x,y);
                ipl_basetype d1 = img - minc;
                ipl_basetype d2 = maxc - img;
                ipl_basetype min = (d1 < d2)? d1 : d2;
                min = (min<1.0)? min : 1.0;
                min = (min>0.0)? min : 0.0;
                newplane->p(x,y) = min;
            }
        }
        delete average;
    }
    return true;
}

IPLData* IPLMorphologicalEdge::getResultData( int )
{
    return _result;
}
