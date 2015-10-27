//#############################################################################
//
//  This file is part of ImagePlay.
//
//  ImagePlay is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ImagePlay is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ImagePlay.  If not, see <http://www.gnu.org/licenses/>.
//
//#############################################################################

#include "IPLLoadVideo.h"

void IPLLoadVideo::init()
{
    // init
    _result         = NULL;

    // basic settings
    setClassName("IPLLoadVideo");
    setTitle("Load Video");
    setDescription("Opens and plays back a video file. The frame rate depends on the complexity of your process chain, the next frame is captured only after all processing of the previous frame is finished.");
    setCategory(IPLProcess::CATEGORY_IO);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);
    setIsSource(true);
    setIsSequence(true);

    // inputs and outputs
    addOutput("Image", IPLData::IMAGE_COLOR);

    addProcessPropertyUnsignedInt("trigger", "Trigger Image", "", 0, IPL_WIDGET_BUTTON);
    addProcessPropertyBool("continuous", "Run continuously", "", true);

    // all properties which can later be changed by gui
    /*addProcessPropertyInt("width", "Width", "", 640, IPL_WIDGET_SLIDER, 640, 1920);
    addProcessPropertyInt("height", "Height", "", 480, IPL_WIDGET_SLIDER, 480, 1080);
    addProcessPropertyInt("brightness", "Brightness", "", 128, IPL_WIDGET_SLIDER, 0, 255);
    addProcessPropertyInt("contrast", "Contrast", "", 128, IPL_WIDGET_SLIDER, 0, 255);
    addProcessPropertyDouble("exposure", "Exposure", "", 0.5, IPL_WIDGET_SLIDER, 0.0, 1.0);*/

    addProcessPropertyString("path", "Video File", "", _path, IPL_WIDGET_FILE_OPEN);
}

void IPLLoadVideo::destroy()
{
    // the camera should be deinitialized automatically in VideoCapture destructor
    //IPLLoadVideoIO::release();

    delete _result;
}

bool IPLLoadVideo::processInputData(IPLImage*, int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    if (_continuous = getProcessPropertyBool("continuous"))
    {
        setIsSequence(true);
    }
    else
    {
        setIsSequence(false);
    }


    // get properties
    _path = getProcessPropertyString("path");
    /*int height = getProcessPropertyInt("height");
    int brightness = getProcessPropertyInt("brightness");
    int contrast = getProcessPropertyInt("contrast");
    double exposure = getProcessPropertyFloat("exposure");*/

    // set camera properties
    /*_camera->set(CV_CAP_PROP_FRAME_WIDTH, width);
    _camera->set(CV_CAP_PROP_FRAME_HEIGHT, height);
    _camera->set(CV_CAP_PROP_BRIGHTNESS, brightness);
    _camera->set(CV_CAP_PROP_CONTRAST, contrast);
    _camera->set(CV_CAP_PROP_EXPOSURE, exposure);*/

    notifyProgressEventHandler(-1);

    //delete _camera;
    //_camera = NULL;

    _result = IPLVideoIO::grabFrame(_path, !_continuous);

    // if we didn't get a frame
    if(!_result)
    {
        addError("Video File not available.");
        return false;
    }

    // collect information
    std::stringstream s;
    /*s << "<b>Width: </b>" << IPLLoadVideoIO::camera()->get(CV_CAP_PROP_FRAME_WIDTH) << "\n";
    s << "<b>Height: </b>" << IPLLoadVideoIO::camera()->get(CV_CAP_PROP_FRAME_HEIGHT) << "\n";
    s << "<b>Brightness: </b>" << IPLLoadVideoIO::camera()->get(CV_CAP_PROP_BRIGHTNESS) << "\n";
    s << "<b>Contrast: </b>" << IPLLoadVideoIO::camera()->get(CV_CAP_PROP_CONTRAST) << "\n";
    s << "<b>Saturation: </b>" << IPLLoadVideoIO::camera()->get(CV_CAP_PROP_SATURATION) << "\n";
    s << "<b>Hue: </b>" << IPLLoadVideoIO::camera()->get(CV_CAP_PROP_HUE) << "\n";
    s << "<b>Gain: </b>" << IPLLoadVideoIO::camera()->get(CV_CAP_PROP_GAIN) << "\n";
    s << "<b>Exposure: </b>" << IPLLoadVideoIO::camera()->get(CV_CAP_PROP_EXPOSURE) << "\n";
    s << "<b>GUID: </b>" << IPLLoadVideoIO::camera()->get(CV_CAP_PROP_GUID) << "";*/

    addInformation(s.str());

    return true;
}

IPLImage *IPLLoadVideo::getResultData(int)
{
    return _result;
}

void IPLLoadVideo::afterProcessing()
{
    notifyPropertyChangedEventHandler();
}
