#include "IPLCanvasSize.h"

void IPLCanvasSize::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLCanvasSize");
    setTitle("Resize Canvas");
    setKeywords("crop");
    setCategory(IPLProcess::CATEGORY_GEOMETRY);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLData::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("width",      "Width", "", IPL_INT_SLIDER, 512, 1, 4096);
    addProcessPropertyInt("height",     "Height", "", IPL_INT_SLIDER, 512, 1, 4096);
    addProcessPropertyInt("anchor",     "Anchor:Top Left|Top|Top Right|Left|Center|Right|Bottom Left|Bottom|Bottom Right", "", IPL_INT_COMBOBOX, 4);
    addProcessPropertyColor("color",    "Background", "If extending the canvas, the background is filled with this color", IPL_COLOR_RGB, IPLColor(0,0,0));
}

void IPLCanvasSize::destroy()
{
    delete _result;
}

bool IPLCanvasSize::processInputData(IPLImage* image , int, bool useOpenCV)
{
    // delete previous result
    delete _result;
    _result = NULL;
    int width  = image->width();
    int height = image->height();

    _result = new IPLImage(image->type(), width, height);

    // get properties
    int         new_width   = getProcessPropertyInt("width");
    int         new_height  = getProcessPropertyInt("height");
    IPLColor    color       = getProcessPropertyColor("color");
    int         anchor      = getProcessPropertyInt("anchor");

    _result = new IPLImage(image->type(), new_width, new_height);

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = _result->getNumberOfPlanes();

    //Anchor:Top Left|Top|Top Right|Left|Center|Right|Bottom Left|Bottom|Bottom Right

    // Top Left
    int offset_x = 0;
    int offset_y = 0;
    if(anchor == 1)
    {
        // Top
        offset_x = (new_width-width) * 0.5;
        offset_y = 0;
    }
    else if(anchor == 2)
    {
        // Top Right
        offset_x = new_width-width;
        offset_y = 0;
    }
    else if(anchor == 3)
    {
        // Left
        offset_x = 0;
        offset_y = (new_height-height) * 0.5;
    }
    else if(anchor == 4)
    {
        // Center
        offset_x = (new_width-width) * 0.5;
        offset_y = (new_height-height) * 0.5;
    }
    else if(anchor == 5)
    {
        // Right
        offset_x = new_width-width;
        offset_y = (new_height-height) * 0.5;
    }
    else if(anchor == 6)
    {
        // Bottom Left
        offset_x = 0;
        offset_y = new_height-height;
    }
    else if(anchor == 7)
    {
        // Bottom
        offset_x = (new_width-width) * 0.5;
        offset_y = new_height-height;
    }
    else if(anchor == 8)
    {
        // Bottom Right
        offset_x = new_width-width;
        offset_y = new_height-height;
    }

    if(nrOfPlanes == 1)
    {
        addWarning("For grayscale images, the red slider is used as background value.");
    }

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );
        ipl_basetype background = 0.0;

        if(planeNr == 0)
            background = color.red();
        else if(planeNr == 1)
            background = color.green();
        if(planeNr == 2)
            background = color.blue();

        for(int y=0; y<new_height; y++)
        {
            // progress
           notifyProgressEventHandler(100*progress++/maxProgress);

            for(int x=0; x<new_width; x++)
            {
                int from_x = x - offset_x;
                int from_y = y - offset_y;

                // check if inside source image
                if(from_x < 0 || from_y < 0 || from_x > plane->width() || from_y > plane->height())
                {
                    newplane->p(x, y) = background;
                }
                else
                {
                    newplane->p(x, y) = plane->cp(from_x, from_y);
                }
            }
        }
    }

    return true;
}

IPLData* IPLCanvasSize::getResultData(int index)
{
    return _result;
}
