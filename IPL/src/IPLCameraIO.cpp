#include "IPLCameraIO.h"

cv::VideoCapture* IPLCameraIO::_camera = NULL;
IPLImage* IPLCameraIO::_lastFrame = NULL;

IPLImage* IPLCameraIO::grabFrame()
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

    _lastFrame = new IPLImage(frame);

    return _lastFrame;
}

void IPLCameraIO::release()
{
    delete _camera;
    _camera = NULL;
}
