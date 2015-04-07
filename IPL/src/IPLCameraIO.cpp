#include "IPLCameraIO.h"

cv::VideoCapture*   IPLCameraIO::_camera    = NULL;
IPLImage*           IPLCameraIO::_lastFrame = NULL;

IPLImage* IPLCameraIO::grabFrame(bool forcedCapture/* = false*/)
{
    // connect camera once
    if(!_camera)
    {
        _camera = new cv::VideoCapture(0);
    }

    if(!_camera->isOpened())
    {
        return NULL;
    }

    // get next frame
    cv::Mat frame;
    *_camera >> frame;

    // workaround: force opencv to grab a new frame
    if(forcedCapture)
        *_camera >> frame;


    _lastFrame = new IPLImage(frame);

    return _lastFrame;
}

cv::VideoCapture *IPLCameraIO::camera()
{
    return _camera;
}

void IPLCameraIO::release()
{
    if(_camera)
        _camera->release();
    //delete _camera;
    //_camera = NULL;
}
