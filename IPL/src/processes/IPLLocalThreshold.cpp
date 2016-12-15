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

#include "IPLLocalThreshold.h"

void IPLLocalThreshold::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLLocalThreshold");
    setTitle("Local Threshold");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);
    setDescription("Niblack's Local Average Threshold");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", 3, IPL_WIDGET_SLIDER_ODD, 3, 9);
    addProcessPropertyDouble("aboveMean", "Above Mean", "", 0.5, IPL_WIDGET_SLIDER, 0.0, 9.0);
}

void IPLLocalThreshold::destroy()
{
    delete _result;
}

bool IPLLocalThreshold::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();

    if(image->type() == IPL_IMAGE_GRAYSCALE)
        _result = new IPLImage(IPL_IMAGE_BW, width, height);
    else
        _result = new IPLImage(image->type(), width, height);

    // get properties
    int window = getProcessPropertyInt("window");
    float aboveMean = getProcessPropertyDouble("aboveMean");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );

        int w2 = window/2;
        double area = (double)window*(double)window;

        for(int y=w2; y < height-w2; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);
            for(int x=w2; x < width-w2; x++)
            {
                double localMean = 0.0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        localMean += (double)plane->p(x+kx,y+ky);
                    }
                }
                localMean /= area;
                double deviation = 0.0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        double diff =  (double)plane->p(x+kx, y+ky) - localMean;
                        deviation += diff * diff;
                    }
                }
                deviation = sqrt( deviation / area );
                double T = (localMean + aboveMean*deviation);

                newplane->p(x,y) = (plane->p(x,y) >= T) ? 1.0 : 0.0;
            }
        }
    }
    return true;
}

IPLData* IPLLocalThreshold::getResultData( int )
{
    return _result;
}
