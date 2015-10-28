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

#include "IPLLaplaceOfGaussian.h"

void IPLLaplaceOfGaussian::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLLaplaceOfGaussian");
    setTitle("Laplacian of Gaussian");
    setKeywords("LoG, lowpass filter");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", 3, IPL_WIDGET_SLIDER_ODD, 3, 9);
    addProcessPropertyDouble("deviation", "Std. Deviation", "", 1, IPL_WIDGET_SLIDER, 1, 10);
    addProcessPropertyBool("zeroCrossing", "Zero Crossing", "", false, IPL_WIDGET_CHECKBOXES);
}

void IPLLaplaceOfGaussian::destroy()
{
    delete _result;
}

bool IPLLaplaceOfGaussian::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

    // get properties
    int window = getProcessPropertyInt("window");
    double deviation = getProcessPropertyDouble("deviation");
    bool zeroCrossing = getProcessPropertyBool("zeroCrossing");

    int w2 = window/2;
    int area = window*window;
    double* w = new double [area];
    double W = 0.0;
    int index = 0;
    double sigma_2 = deviation*deviation;

    for( int y = -w2; y <= w2; y++ )
    {
        for( int x = -w2; x <= w2; x++)
        {
            double r_2 = ((double)x*(double)x+(double)y*(double)y);
            w[index] = 1.0/sigma_2*(r_2/sigma_2-2.0)*exp( -r_2 / (2.0*sigma_2));
            W += w[index++];
        }
    }
    // normalize to zero
    for( int i=0; i<area; i++ )
        w[i] -= W/area;

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );
        IPLImagePlane* tmpPlane = new IPLImagePlane( width, height );

        for(int x=0; x<width; x++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int y=0; y<height; y++)
            {
                double sum = 0;
                int i = 0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        double img = (double) plane->bp(x+kx, y+ky);
                        sum += (img * w[i++]);
                    }
                }
                tmpPlane->p(x,y) = sum;
            }
        }
        if( zeroCrossing )
        {
            for(int x=1; x<width-1; x++)
            {
                for(int y=1; y<height-1; y++)
                {
                    newplane->p(x,y) = 0;
                    if( tmpPlane->p(x-1,y)*tmpPlane->p(x+1,y) < 0.0 ) newplane->p(x,y) = 1.0;
                    if( tmpPlane->p(x,y-1)*tmpPlane->p(x,y+1) < 0.0 ) newplane->p(x,y) = 1.0;
                    if( tmpPlane->p(x-1,y-1)*tmpPlane->p(x+1,y+1) < 0.0 ) newplane->p(x,y) = 1.0;
                    if( tmpPlane->p(x-1,y+1)*tmpPlane->p(x+1,y-1) < 0.0 ) newplane->p(x,y) = 1.0;
                }
            }
        }
        else
        {
            for(int x=0; x<width; x++)
            {
                for(int y=0; y<height; y++)
                {
                    if( tmpPlane->p(x,y) > 1.0 ) newplane->p(x,y) = 1.0;
                    else if( tmpPlane->p(x,y) < 0.0 ) newplane->p(x,y) = 0.0;
                    else newplane->p(x,y) = tmpPlane->p(x,y);
                }
            }
        }
        delete tmpPlane;
    }

    delete [] w;
    return true;
}

IPLData* IPLLaplaceOfGaussian::getResultData( int )
{
    return _result;
}
