#include "IPLFlipImage.h"

void IPLFlipImage::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLFlipImage");
    setTitle("Flip Image");
    setCategory(IPLProcess::CATEGORY_GEOMETRY);
    setDescription("Flip an image horizontally or vertically.");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLData::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("direction", "Direction:Horizontal|Vertical", "", 0, IPL_WIDGET_RADIOBUTTONS);
}

void IPLFlipImage::destroy()
{
    delete _result;
}

bool IPLFlipImage::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage(image->type(), width, height);

    // get properties
    int direction = getProcessPropertyInt("direction");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        notifyProgressEventHandler(100*progress++/maxProgress);

        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );
        for(int x=0; x < _result->width(); x++)
        {
            for(int y=0; y < _result->height(); y++)
             {
                if(direction == 0)
                {
                    // horizontal flip
                    int x2 = width - x - 1;
                    newplane->p(x,y) = plane->p(x2,y);
                }
                else
                {
                    // vertical flip
                    int y2 = height - y - 1;
                    newplane->p(x,y) = plane->p(x,y2);
                }

             }
        }
    }

    return true;
}

IPLImage* IPLFlipImage::getResultData( int )
{
    return _result;
}
