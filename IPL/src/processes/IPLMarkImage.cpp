#include "IPLMarkImage.h"

void IPLMarkImage::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLMarkImage");
    setTitle("Mark Image");
    setCategory(IPLProcess::CATEGORY_POINTOPERATIONS);

    // inputs and outputs
    addInput("Grayscale Image", IPLData::IMAGE_GRAYSCALE);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyDouble("min", "Min", "", IPL_DOUBLE_SLIDER, 0.4, 0.0, 1.0);
    addProcessPropertyDouble("max", "Max", "", IPL_DOUBLE_SLIDER, 0.6, 0.0, 1.0);
    addProcessPropertyInt("mode", "Mode:Show Background|Mask Only", "", IPL_INT_COMBOBOX, 0);
    addProcessPropertyColor("color", "Color", "", IPL_COLOR_RGB, IPLColor(1.0,0.0,0.0));
}

void IPLMarkImage::destroy()
{
    delete _result;
}

bool IPLMarkImage::processInputData(IPLImage* image , int, bool useOpenCV)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage(IPLImage::IMAGE_COLOR, width, height );

    // get properties
    float min = getProcessPropertyDouble("min");
    float max = getProcessPropertyDouble("max");
    IPLColor color = getProcessPropertyColor("color");
    int mode = getProcessPropertyInt("mode");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();

    IPLImagePlane* plane = image->plane(0);
    IPLImagePlane* r = _result->plane(0);
    IPLImagePlane* g = _result->plane(1);
    IPLImagePlane* b = _result->plane(2);

    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);
        for(int x=0; x<width; x++)
        {
            if(plane->p(x,y) >= min && plane->p(x,y) <= max)
            {
                r->p(x,y) = color.red();
                g->p(x,y) = color.green();
                b->p(x,y) = color.blue();
            }
            else
            {
                r->p(x,y) = mode ? 0.0 : plane->p(x,y);
                g->p(x,y) = mode ? 0.0 : plane->p(x,y);
                b->p(x,y) = mode ? 0.0 : plane->p(x,y);
            }
        }
    }
    return true;
}

IPLData* IPLMarkImage::getResultData( int )
{
    return _result;
}
