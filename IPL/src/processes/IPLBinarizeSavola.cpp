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

#include "IPLBinarizeSavola.h"

void IPLBinarizeSavola::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLBinarizeSavola");
    setTitle("Local Threshold (Savola)");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);
    setDescription("Local Threshold as proposed by Savola et. al.");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window Size", "", 3, IPL_WIDGET_SLIDER_ODD, 3, 9);
    addProcessPropertyDouble("aboveMean", "Above Mean", "", 0.0, IPL_WIDGET_SLIDER, 0.0, 1.0);
}

void IPLBinarizeSavola::destroy()
{
    delete _result;
}

bool IPLBinarizeSavola::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;

    int width  = image->width();
    int height = image->height();

    _result = new IPLImage(image->type(), width, height);

    // get properties
    int window = getProcessPropertyInt("window");
    double aboveMean = getProcessPropertyDouble("aboveMean");


    IPLImage* mean = new IPLImage(image->type(), width, height);
    IPLImage* deviation = new IPLImage(image->type(), width, height);

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );

        int w2 = window/2;
        float area = (float)(w2*2)*(float)(w2*2);
        float minI = FLT_MAX;
        float maxDeviation = 0.0;

        for(int y=0; y<height; y++)
        {
            // progress
           notifyProgressEventHandler(100*progress++/maxProgress);

            for(int x=0; x<width; x++)
            {
                if( plane->p(x,y) < minI ) minI = plane->p(x,y);
                float localMean = 0.0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        localMean += (float)plane->cp(x+kx,y+ky);
                    }
                }
                localMean /= area;
                mean->plane(planeNr)->p(x,y) = localMean;

                float dev = 0.0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        float diff =  (float)plane->cp(x+kx, y+ky) - localMean;
                        dev += diff * diff;
                    }
                }
                dev = sqrt( dev / area );
                deviation->plane(planeNr)->p(x,y) = dev;
                if( dev > maxDeviation ) maxDeviation = dev;
            }

            for(int x=w2; x<width-w2; x++)
            {
                for(int y=w2; y<height-w2; y++)
                {
                    float alpha = 1.0 - deviation->plane(planeNr)->p(x,y) / maxDeviation;
                    int T = (int) ( mean->plane(planeNr)->p(x,y) - aboveMean * alpha *( mean->plane(planeNr)->p(x,y) - minI ) );
                    newplane->p(x,y) = ( plane->p(x,y) >= T ) ? 0.0 : 1.0;
                }
            }
        }
    }

    return true;
}

IPLData* IPLBinarizeSavola::getResultData(int)
{
    return _result;
}
