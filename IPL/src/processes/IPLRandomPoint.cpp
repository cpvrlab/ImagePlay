#include "IPLRandomPoint.h"

void IPLRandomPoint::init()
{
    // init
    _image = NULL;
    _point = NULL;

    // basic settings
    setClassName("IPLRandomPoint");
    setTitle("Random Point");
    setCategory(IPLProcess::CATEGORY_OBJECTS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLData::IMAGE_COLOR);
    addOutput("Random Point", IPLImage::POINT);

    // properties
    addProcessPropertyDouble("threshold", "Threshold", "0.0 < threshold < 1.0", IPL_DOUBLE_SLIDER, 0.5, 0.0, 1.0);
}

void IPLRandomPoint::destroy()
{
    delete _image;
    delete _point;
}

bool IPLRandomPoint::processInputData(IPLImage* image, int, bool)
{
    // delete previous result
    delete _image;
    _image = new IPLImage(*image);

    int width = image->width();
    int height = image->height();

    delete _point;
    _point = new IPLPoint;

    srand(time(0));
    int x = rand() % width;
    int y = rand() % height;

    _point->setX(x);
    _point->setY(y);

    return true;
}

IPLData* IPLRandomPoint::getResultData( int index )
{
    if(index == 0)
        return _image;
    else
        return _point;
}
