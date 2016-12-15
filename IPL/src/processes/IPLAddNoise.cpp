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

#include "IPLAddNoise.h"

void IPLAddNoise::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLAddNoise");
    setTitle("Add Noise");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);
    setKeywords("gaussian, impulsive");
    setDescription("Add impulse, Gaussian noise (additive or multiplicative) to an image.");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);

    // properties
    addProcessPropertyInt("type", "Type:Impulsive|Gaussian", "impl|gauss", 0, IPL_WIDGET_GROUP);
    addProcessPropertyDouble("impl_probability", "Probability", "", 0.2, IPL_WIDGET_SLIDER, 0.0, 0.99);
    addProcessPropertyDouble("impl_amplitude", "Amplitude", "", 0.2, IPL_WIDGET_SLIDER, 0.0, 1.0);
    addProcessPropertyDouble("gauss_stdDev", "Std. Deviation", "", 0.1, IPL_WIDGET_SLIDER, 0.0, 1.0);
    addProcessPropertyInt("gauss_type", "Type:Multiplicative|Additive", "", 0, IPL_WIDGET_RADIOBUTTONS);
}

void IPLAddNoise::destroy()
{
    delete _result;
}

bool IPLAddNoise::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    int width = image->width();
    int height = image->height();

    // delete previous result
    delete _result;
    _result = new IPLImage( image->type(), width, height );

    // get properties
    int type = getProcessPropertyInt("type");
    float probability = getProcessPropertyDouble("impl_probability");
    float amplitude = getProcessPropertyDouble("impl_amplitude");
    float stdDev = getProcessPropertyDouble("gauss_stdDev");
    int gaussType = getProcessPropertyInt("gauss_type");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    std::default_random_engine generator;
    generator.seed(time(0));
    std::normal_distribution<double> distribution(1.0, stdDev);

    time_t t;
    srand((unsigned) time(&t));

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );
        for(int y=0; y<height; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);
            for(int x=0; x<width; x++)
            {
                ipl_basetype p = plane->p(x,y);

                // impulsive noise
                if(type == 0)
                {
                    int qneg = (int)( (double)probability * RAND_MAX) ;
                    int qpos = RAND_MAX - qneg;
                    int r = rand();
                    if( r > qpos ) { p += amplitude; p = (p>1.0) ? 1.0 : p; }
                    if( r < qneg ) { p -= amplitude; p = (p<0.0) ? 0.0 : p; }
                    newplane->p(x,y) = p;
                }
                else
                {
                    // gaussian noise
                    double rand = distribution(generator);

                    if(gaussType == 0) // multiplicative
                    {
                        p *= rand;
                    }
                    else                 // additive
                    {
                        p += rand - 1.0;
                    }
                    p = (p>1.0) ? 1.0 : p;
                    p = (p<0.0) ? 0.0 : p;

                    newplane->p(x,y) = p;
                }
            }
        }
    }
    return true;
}

IPLData* IPLAddNoise::getResultData( int )
{
    return _result;
}
