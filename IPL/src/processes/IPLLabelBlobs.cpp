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

    // inputs and outputs
    addInput("Binary Image", IPLData::IMAGE_BW);
    addOutput("Objects", IPLImage::IMAGE_GRAYSCALE);
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

    int width = image->width();
    int height = image->height();
    _result = new IPLImage(IPLImage::IMAGE_GRAYSCALE, width, height );

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();

    IPLImagePlane* plane = image->plane(0);
    IPLImagePlane* newplane = _result->plane(0);

    // label increment 0.01;
    float label = 0.01;
    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);
        for(int x=0; x<width; x++)
        {
            if(plane->p(x,y))
                labelBlob(plane, newplane, x, y, label += 0.01);
        }
    }
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
