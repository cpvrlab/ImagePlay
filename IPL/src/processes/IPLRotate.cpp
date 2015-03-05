#include "IPLRotate.h"

void IPLRotate::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLRotate");
    setTitle("Rotate/Zoom Image");
    setCategory(IPLProcess::CATEGORY_GEOMETRY);
    setOpenCVSupport(IPLProcess::OPENCV_ONLY);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLData::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("angle", "Angle", "", IPL_INT_SLIDER, 0, -180, 180);
    addProcessPropertyDouble("scale", "Scaling", "", IPL_DOUBLE_SLIDER, 1.0, 0.1, 10.0);
    addProcessPropertyPoint("center", "Center", "", IPL_POINT, IPLPoint(0,0));
    addProcessPropertyInt("interpolation", "Interpolation:Nearest|Linear|Area|Cubic|Lanczos4", "", IPL_INT_COMBOBOX, 0, 0, 4);
}

void IPLRotate::destroy()
{
    delete _result;
}

bool IPLRotate::processInputData(IPLImage* image , int, bool useOpenCV)
{
    // delete previous result
    delete _result;
    _result = NULL;

    // get properties
    int angle = getProcessPropertyInt("angle");
    double scale = getProcessPropertyDouble("scale");
    int interpolation = getProcessPropertyInt("interpolation");
    IPLPoint center = getProcessPropertyPoint("center");

    // Compute a rotation matrix with respect to the center of the image
    //cv::Point centerPoint = cv::Point( image->width()/2, image->height()/2 );
    cv::Point centerPoint = cv::Point(center.x(), center.y());

    /*std::stringstream s;
    s << "<b>Original Aspect Ratio</b>: " << aspectRatio << "\n";
    s << "<b>New Aspect Ratio</b>: " << newAspectRatio << "\n";
    s << "<b>Height</b>: " << height << "\n";
    s << "<b>Width</b>: " << width;
    addInformation(s.str());*/

    _result = new IPLImage(image->type(), image->width(), image->height());

    cv::Mat result;

    // Get the rotation matrix with the specifications above
    cv::Mat rot_mat = getRotationMatrix2D(centerPoint, angle, scale);

    // Rotate the warped image
    cv::warpAffine(image->toCvMat(), result, rot_mat, result.size(), interpolation);

    delete _result;
    _result = new IPLImage(result);

    return true;
}

IPLData* IPLRotate::getResultData(int index)
{
    return _result;
}
