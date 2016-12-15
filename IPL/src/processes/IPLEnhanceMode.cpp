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

#include "IPLEnhanceMode.h"

void IPLEnhanceMode::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLEnhanceMode");
    setTitle("Enhance Mode");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);
    setDescription("Enhance the local mode.");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", 3, IPL_WIDGET_SLIDER_ODD, 3, 15);
}

void IPLEnhanceMode::destroy()
{
    delete _result;
}

bool IPLEnhanceMode::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

    // get properties
    float window = getProcessPropertyInt("window");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    int w2 = window/2;
    int area = window * window;

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );


        int H[256], W[256];
        for( int z=0; z<256; z++ )
            H[z] = 0;

        for(int y=w2; y<height-w2; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);
            for(int x=w2; x<width-w2; x++)
            {
                // operator
                for( int ky=-w2; ky <= w2; ky++)
                {
                    for( int kx=-w2; kx <= w2; kx++)
                    {
                        int index = (int) (plane->p(x+kx, y+ky) * FACTOR_TO_UCHAR);
                        H[index]++;
                    }
                }

                // bucketsort
                int z = 0;
                for( int u=0; u<256; u++ )
                {
                    if(H[u] > 0)
                    {
                        while( H[u] > 0)
                        {
                            W[z++] = u;
                            H[u]--;
                        }
                    }
                }

                int qmin = INT_MAX;
                int u;
                for( int s=0; s<area; s++ )
                {
                    u = std::max( 0, s-w2-1);
                    int v = std::min( area-1, s+w2+1);
                    int q = 0;
                    for( int z=u; z<v; z++ )
                        if( s != z )
                            q += abs( (W[s] - W[z]) / (s - z) );
                    q /= (v - u);
                    if ( q < qmin )
                    {
                        u = s;
                        qmin = q;

                    }
                }
                newplane->p(x,y) = (float) W[u] * FACTOR_TO_FLOAT;
            }
        }
    }
    return true;
}

IPLData* IPLEnhanceMode::getResultData( int )
{
    return _result;
}
