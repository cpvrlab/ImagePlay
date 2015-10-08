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
}
