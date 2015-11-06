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

#include "IPLLabelBlobs.h"

void IPLLabelBlobs::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLLabelBlobs");
    setTitle("Label Blobs");
    setCategory(IPLProcess::CATEGORY_OBJECTS);
    setKeywords("objects");
    setDescription("Label blobs (connected components) of binary input image.");

    // properties
    addProcessPropertyInt("labelCount", "Number of Labels", "Only maximum of labels is counted. This determines the intensity increments of the output image.", 1024, IPL_WIDGET_SLIDER, 1, 4096);

    // inputs and outputs
    addInput("Binary Image", IPL_IMAGE_BW);
    addOutput("Objects", IPL_IMAGE_GRAYSCALE);
}

void IPLLabelBlobs::destroy()
{
    delete _result;
}

bool IPLLabelBlobs::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    // we need a working copy
    IPLImage* input = new IPLImage(*image);

    int width = image->width();
    int height = image->height();
    _result = new IPLImage(IPL_IMAGE_GRAYSCALE, width, height );

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();

    IPLImagePlane* plane = input->plane(0);
    IPLImagePlane* newplane = _result->plane(0);

    int labelCount = getProcessPropertyInt("labelCount");
    float labelIncrement = 1.0f/labelCount;
    float label = 0.01f;
    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);
        for(int x=0; x<width; x++)
        {
            // if the label exceeds 1 leave it white.
            if(label + labelIncrement > 1.0f)
                newplane->p(x, y) = 1.0;
            else if(plane->p(x,y))
                labelBlob(plane, newplane, x, y, label += labelIncrement);
        }
    }
    delete input;
    return true;
}

void IPLLabelBlobs::labelBlob(IPLImagePlane* inPlane, IPLImagePlane* outPlane, int x, int y, float label)
{
    int width = inPlane->width();
    int height = inPlane->height();
    Queue queue;
    queue.push_back(Pixel(x, y));
    while( !queue.empty() )
    {
        Pixel c = queue.front(); queue.pop_front();
        if( c.x<width && c.x >=0 && c.y<height && c.y>=0 )
        {
            if( inPlane->p(c.x,c.y) )
            {
                inPlane->p(c.x,c.y) = 0;
                outPlane->p(c.x,c.y) = label;
                queue.push_back( Pixel( c.x+1, c.y ) );
                queue.push_back( Pixel( c.x, c.y+1 ) );
                queue.push_back( Pixel( c.x-1, c.y ) );
                queue.push_back( Pixel( c.x, c.y-1 ) );
                queue.push_back( Pixel( c.x+1, c.y+1 ) );
                queue.push_back( Pixel( c.x-1, c.y+1 ) );
                queue.push_back( Pixel( c.x-1, c.y-1 ) );
                queue.push_back( Pixel( c.x+1, c.y-1 ) );
            }
        }
    }
}

IPLData* IPLLabelBlobs::getResultData( int )
{
    return _result;
}
