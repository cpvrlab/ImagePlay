#include "IPLCamera.h"

void IPLCamera::init()
{
    // init
    _result         = NULL;

    // basic settings
    setClassName("IPLCamera");
    setTitle("Capture Camera");
    setDescription("Opens and captures images from the default camera which is connected to your computer. The frame rate depends on the complexity of your process chain, the next frame is captured only after all processing of the previous frame is finished.");
    setCategory(IPLProcess::CATEGORY_IO);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);
    setIsSource(true);
    setIsSequence(true);

    // inputs and outputs
    addOutput("Image", IPLData::IMAGE_COLOR);

    // all properties which can later be changed by gui
    addProcessPropertyInt("width", "Width", "", IPL_INT_SLIDER, 640, 640, 1920);
    addProcessPropertyInt("height", "Height", "", IPL_INT_SLIDER, 480, 480, 1080);
    addProcessPropertyInt("brightness", "Brightness", "", IPL_INT_SLIDER, 128, 0, 255);
    addProcessPropertyInt("contrast", "Contrast", "", IPL_INT_SLIDER, 128, 0, 255);
    addProcessPropertyDouble("exposure", "Exposure", "", IPL_DOUBLE_SLIDER, 0.5, 0.0, 1.0);
}

void IPLCamera::destroy()
{
    // the camera should be deinitialized automatically in VideoCapture destructor
    //IPLCameraIO::release();

    delete _result;
}

bool IPLCamera::processInputData(IPLImage*, int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    // get properties
    /*int width = getProcessPropertyInt("width");
    int height = getProcessPropertyInt("height");
    int brightness = getProcessPropertyInt("brightness");
    int contrast = getProcessPropertyInt("contrast");
    double exposure = getProcessPropertyFloat("exposure");*/

    // set camera properties
    /*_camera->set(CV_CAP_PROP_FRAME_WIDTH, width);
    _camera->set(CV_CAP_PROP_FRAME_HEIGHT, height);
    _camera->set(CV_CAP_PROP_BRIGHTNESS, brightness);
    _camera->set(CV_CAP_PROP_CONTRAST, contrast);
    _camera->set(CV_CAP_PROP_EXPOSURE, exposure);*/

    //notifyProgressEventHandler(99);

    //delete _camera;
    //_camera = NULL;

    _result = IPLCameraIO::grabFrame();

    // if we didn't get a frame
    if(!_result)
    {
        addError("Camera not available.");
        return false;
    }

    // collect information
    std::stringstream s;
    s << "<b>Width: </b>" << IPLCameraIO::camera()->get(CV_CAP_PROP_FRAME_WIDTH) << "\n";
    s << "<b>Height: </b>" << IPLCameraIO::camera()->get(CV_CAP_PROP_FRAME_HEIGHT) << "\n";
    s << "<b>Brightness: </b>" << IPLCameraIO::camera()->get(CV_CAP_PROP_BRIGHTNESS) << "\n";
    s << "<b>Contrast: </b>" << IPLCameraIO::camera()->get(CV_CAP_PROP_CONTRAST) << "\n";
    s << "<b>Saturation: </b>" << IPLCameraIO::camera()->get(CV_CAP_PROP_SATURATION) << "\n";
    s << "<b>Hue: </b>" << IPLCameraIO::camera()->get(CV_CAP_PROP_HUE) << "\n";
    s << "<b>Gain: </b>" << IPLCameraIO::camera()->get(CV_CAP_PROP_GAIN) << "\n";
    s << "<b>Exposure: </b>" << IPLCameraIO::camera()->get(CV_CAP_PROP_EXPOSURE) << "\n";
    s << "<b>GUID: </b>" << IPLCameraIO::camera()->get(CV_CAP_PROP_GUID) << "";

    addInformation(s.str());

    return true;
}

IPLImage *IPLCamera::getResultData(int)
{
    return _result;
}
