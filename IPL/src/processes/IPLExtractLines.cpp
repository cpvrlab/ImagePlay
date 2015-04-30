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

#include "IPLExtractLines.h"

void IPLExtractLines::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLExtractLines");
    setTitle("Extract Lines");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", 3, IPL_WIDGET_SLIDER, 3, 15);
}

void IPLExtractLines::destroy()
{
    delete _result;
}

bool IPLExtractLines::processInputData(IPLImage* image , int, bool useOpenCV)
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
                int m[4], ma, mi;
                for( int z=0; z<4; z++ )
                    m[z] = 0;
                for( int z=-w2; z <= w2; z++ )
                {
                    m[0] += plane->p(x+z, y) * FACTOR_TO_UCHAR;
                    m[1] += plane->p(x, y+z) * FACTOR_TO_UCHAR;
                    m[2] += plane->p(x+z, y+z) * FACTOR_TO_UCHAR;
                    m[3] += plane->p(x+z, y-z) * FACTOR_TO_UCHAR;
                }
                ma = m[0];
                for( int z=1; z<4; z++ )
                    ma = (ma>m[z])? ma : m[z];
                if( ma == 0 )
                    newplane->p(x,y) = 0;
                else
                {
                    mi = m[0];
                    for( int z=1; z<4; z++ )
                        mi = (mi<m[z])? mi : m[z];
                    newplane->p(x,y)  = (((double)(ma-mi)/(double)(ma+mi)));
                }
            }
        }
    }
    return true;
}

IPLData* IPLExtractLines::getResultData( int )
{
    return _result;
}
