#include "IPLConvertToGray.h"

void IPLConvertToGray::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLConvertToGray");
    setTitle("Convert to Gray");
    setKeywords("grayscale");
    setDescription("Converts an image with the color format binary, gray-scale, or color to a gray-scale image."
                   "Default weights based on human eye perception:\nR: 0.2125\nG: 0.7154\nB: 0.0721");
    setCategory(IPLProcess::CATEGORY_CONVERSIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Gray Image", IPLData::IMAGE_GRAYSCALE);

    // properties
    addProcessPropertyDouble("weight_r", "Weight R", "", 0.2125, IPL_WIDGET_SLIDER, 0.0, 1.0);
    addProcessPropertyDouble("weight_g", "Weight G", "", 0.7154, IPL_WIDGET_SLIDER, 0.0, 1.0);
    addProcessPropertyDouble("weight_b", "Weight B", "", 0.0721, IPL_WIDGET_SLIDER, 0.0, 1.0);
}

void IPLConvertToGray::destroy()
{
    delete _result;
}

bool IPLConvertToGray::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();

    IPLImagePlane* plane;
    IPLImagePlane* newplane;
    IPLImagePlane* red;
    IPLImagePlane* green;
    IPLImagePlane* blue;

    // properties
    double weight_r = getProcessPropertyDouble("weight_r");
    double weight_g = getProcessPropertyDouble("weight_g");
    double weight_b = getProcessPropertyDouble("weight_b");

    int progress = 0;
    int maxProgress = image->height();

    switch( image->type() )
    {
    case IPLImage::IMAGE_BW:
        _result = new IPLImage( IPLData::IMAGE_GRAYSCALE, width, height );
        plane = image->plane( 0 );
        newplane = _result->plane( 0 );

        for(int y=0; y<height; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int x=0; x<width; x++)
            {
                newplane->p(x,y) = plane->p(x,y);
            }
        }
        break;
    case IPLData::IMAGE_GRAYSCALE:
        _result = new IPLImage( IPLData::IMAGE_GRAYSCALE, width, height );
        plane = image->plane( 0 );
        newplane = _result->plane( 0 );

        for(int y=0; y<height; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int x=0; x<width; x++)
            {
                newplane->p(x,y) = plane->p(x,y);
            }
        }
        break;
    case IPLData::IMAGE_COLOR:
        _result = new IPLImage( IPLData::IMAGE_GRAYSCALE, width, height );
        newplane = _result->plane( 0 );
        red = image->plane( 0 );
        green = image->plane( 1 );
        blue = image->plane( 2 );
        //unsigned char r, b, g;

        for(int y=0; y<height; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int x=0; x<width; x++)
            {
                //r = red->p(x,y);
                //g = green->p(x,y);
                //b = blue->p(x,y);
                //unsigned char pixel = HIBYTE(red->p(x,y) * 77 + green->p(x,y) * 151 + blue->p(x,y) * 28); //!< Fast
                //unsigned char pixel = (unsigned char) (0.2125 * red->p(x,y)) + (0.7154 * green->p(x,y)) + (0.0721 * blue->p(x,y)); //!< Slower
                //unsigned char pixel = (unsigned char) ((r+r+r+b+g+g+g+g)>>3); //!< Even Faster
                //ipl_basetype pixel = (0.2125f * red->p(x,y)) + (0.7154f * green->p(x,y)) + (0.0721f * blue->p(x,y)); //!< Slower
                ipl_basetype pixel = (weight_r * red->p(x,y)) + (weight_g * green->p(x,y)) + (weight_b * blue->p(x,y)); //!< Slower
                pixel = (pixel > 1.0) ? 1.0 : pixel;
                pixel = (pixel < 0.0) ? 0.0 : pixel;
                newplane->p(x,y) = pixel;
            }
        }
        break;
    }

    return true;
}

IPLImage* IPLConvertToGray::getResultData( int )
{
    return _result;
}
