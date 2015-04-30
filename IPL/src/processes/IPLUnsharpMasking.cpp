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

#include "IPLUnsharpMasking.h"

void IPLUnsharpMasking::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLUnsharpMasking");
    setTitle("Unsharp Masking");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", 3, IPL_WIDGET_SLIDER_ODD, 3, 9);
    addProcessPropertyDouble("factor", "Factor", "", 0.3, IPL_WIDGET_SLIDER, 0.1, 0.9);
}

void IPLUnsharpMasking::destroy()
{
    delete _result;
}

bool IPLUnsharpMasking::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

    // get properties
    int window = getProcessPropertyInt("window");
    double factor = getProcessPropertyDouble("factor");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    int w2 = window/2;
    int area = window*window;
    double* w = new double [area];

    double b3 = ((double)(window-1)*(double)(window-1))/9.0;
    double W = 0.0;
    int i = 0;

    for( int ky=-w2; ky <= w2; ky++ )
    {
        for( int kx=-w2; kx <= w2; kx++ )
        {
            w[i] = exp( -((double)kx*(double)kx+(double)ky*(double)ky) / b3);
            W += w[i++];
        }
    }
    W -= w[(area-1)/2];

    //#pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );

        for(int x=0; x<width; x++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int y=0; y<height; y++)
            {
                ipl_basetype sum = 0.0;
                i = 0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        ipl_basetype img = plane->bp(x+kx, y+ky);
                        sum += (img * w[i++]);
                    }
                }
                ipl_basetype img = plane->p(x,y);
                sum -= img * w[(area-1)/2];
                img = 1.0 / (1.0 - factor) * (img - factor*(sum+img)/(W+1.0) );
                img = img > 1.0 ? 1.0 : img;
                img = img < 0.0 ? 0.0 : img;
                newplane->p(x,y) = img;
            }
        }
    }
    return true;
}

IPLData* IPLUnsharpMasking::getResultData( int )
{
    return _result;
}
