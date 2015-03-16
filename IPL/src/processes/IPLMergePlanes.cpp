#include "IPLMergePlanes.h"

void IPLMergePlanes::init()
{
    // init
    _result = NULL;
    _inputA = NULL;
    _inputB = NULL;
    _inputC = NULL;
    _inputType  = 0;

    // basic settings
    setClassName("IPLMergePlanes");
    setTitle("Merge Planes");
    setCategory(IPLProcess::CATEGORY_CONVERSIONS);
    setDescription("Converts color planes into a color image. The input planes may be images in"
                   "the RGB, HSI, HLS, or HSV color system");

    // properties
    addProcessPropertyInt("input_type", "Color Model:RGB|HSV|HSL", "", IPL_INT_RADIOBUTTONS, _inputType);

    // inputs and outputs
    addInput("Plane 1", IPLData::IMAGE_GRAYSCALE);
    addInput("Plane 2", IPLData::IMAGE_GRAYSCALE);
    addInput("Plane 3", IPLData::IMAGE_GRAYSCALE);
    addOutput("Image",  IPLData::IMAGE_COLOR);

}

void IPLMergePlanes::destroy()
{
    delete _result;
    delete _inputA;
    delete _inputB;
    delete _inputC;
}

bool IPLMergePlanes::processInputData(IPLImage* image , int imageIndex, bool)
{
    // save the inputs
    if(imageIndex == 0)
    {
        delete _inputA;
        _inputA = new IPLImage(*image);
    }
    if(imageIndex == 1)
    {
        delete _inputB;
        _inputB = new IPLImage(*image);
    }
    if(imageIndex == 2)
    {
        delete _inputC;
        _inputC = new IPLImage(*image);
    }

    // only continue if we have 2 valid inputs
    if(!(_inputA && _inputB && _inputC))
    {
        return false;
    }

    // get properties
    int inputType = getProcessPropertyInt("input_type");

    int width   = std::max(std::max(_inputA->width(), _inputB->width()), _inputC->width());
    int height  = std::max(std::max(_inputA->height(), _inputB->height()), _inputC->height());

    delete _result;
    _result =  new IPLImage(IPLData::IMAGE_COLOR, width, height);

    int progress = 0;
    int maxProgress = image->height() * _result->getNumberOfPlanes();

    // copy data
    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);

        for(int x=0; x<width; x++)
        {
            // RGB
            float r = _inputA->plane(0)->cp(x, y);
            float g = _inputB->plane(0)->cp(x, y);
            float b = _inputC->plane(0)->cp(x, y);

            // HSV
            if(inputType == 1)
            {
                float rgb[3];
                IPLColor::hsvToRgb(r, g, b, rgb);
                r = rgb[0];
                g = rgb[1];
                b = rgb[2];
            }
            // HSL
            else if(inputType == 2)
            {
                float rgb[3];
                IPLColor::hslToRgb(r, g, b, rgb);
                r = rgb[0];
                g = rgb[1];
                b = rgb[2];
            }
            _result->plane(0)->p(x, y) = r;
            _result->plane(1)->p(x, y) = g;
            _result->plane(2)->p(x, y) = b;
        }
    }

    return true;
}

IPLImage* IPLMergePlanes::getResultData(int)
{
    return _result;
}

