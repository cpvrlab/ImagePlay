#include "IPLResize.h"

void IPLResize::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLResize");
    setTitle("Resize Image");
    setCategory(IPLProcess::CATEGORY_GEOMETRY);
    setOpenCVSupport(IPLProcess::OPENCV_ONLY);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLData::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("mode", "Mode:Absolute|Factor", "Uses either height and width or factor x and factor y", IPL_INT_COMBOBOX, 0, 0, 1);
    addProcessPropertyInt("width", "Width", "", IPL_INT_SLIDER, 512, 1, 4096);
    addProcessPropertyInt("height", "Height", "", IPL_INT_SLIDER, 512, 1, 4096);
    addProcessPropertyDouble("factor_x", "Factor X", "", IPL_DOUBLE_SLIDER, 1.0, 0.1, 10.0);
    addProcessPropertyDouble("factor_y", "Factor Y", "", IPL_DOUBLE_SLIDER, 1.0, 0.1, 10.0);
    addProcessPropertyInt("keep_aspect_ratio", "Keep Aspect Ratio:None|Based on width|Based on height", "", IPL_INT_COMBOBOX, 0, 0, 1);
    addProcessPropertyInt("interpolation", "Interpolation:Nearest|Linear|Area|Cubic|Lanczos4", "", IPL_INT_COMBOBOX, 0, 0, 4);
}

void IPLResize::destroy()
{
    delete _result;
}

bool IPLResize::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    // get properties
    int mode = getProcessPropertyInt("mode");
    int width = getProcessPropertyInt("width");
    int height = getProcessPropertyInt("height");
    double factor_x = getProcessPropertyDouble("factor_x");
    double factor_y = getProcessPropertyDouble("factor_y");
    int interpolation = getProcessPropertyInt("interpolation");
    int keep_aspect_ratio = getProcessPropertyInt("keep_aspect_ratio");

    // calculate height/width based on aspect_ratio
    double aspectRatio = 1.0;
    if(image->height() > 0)
        aspectRatio = (double) image->width()/image->height();
    if(keep_aspect_ratio == 1)
    {
        // calculate height
        height = width / aspectRatio;
        factor_y = factor_x;
    }
    else if(keep_aspect_ratio == 2)
    {
        // calculate width
        width = height * aspectRatio;
        factor_x = factor_y;
    }

    double newAspectRatio = (double) width / height;
    if(mode == 1)
        newAspectRatio = (double) (width*factor_x) / (height*factor_y);

    std::stringstream s;
    s << "<b>Original Aspect Ratio</b>: " << aspectRatio << "\n";
    s << "<b>New Aspect Ratio</b>: " << newAspectRatio << "\n";
    s << "<b>Height</b>: " << height << "\n";
    s << "<b>Width</b>: " << width;
    addInformation(s.str());

    _result = new IPLImage(image->type(), width, height);

    cv::Mat result;
    if(mode == 0)
        cv::resize(image->toCvMat(), result, cv::Size(width, height), 0, 0, interpolation);
    else
        cv::resize(image->toCvMat(), result, cv::Size(0, 0), factor_x, factor_y, interpolation);

    delete _result;
    _result = new IPLImage(result);

    return true;
}

IPLData* IPLResize::getResultData(int)
{
    return _result;
}
