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

#include "IPLVideoIO.h"

cv::VideoCapture*   IPLVideoIO::_video    = NULL;
IPLImage*           IPLVideoIO::_lastFrame = NULL;

IPLImage* IPLVideoIO::grabFrame(std::string fileName, bool forcedCapture/* = false*/)
{
    // connect camera once
    if(!_video)
    {
        _video = new cv::VideoCapture();
    }

    _video->open(fileName);

    if(!_video->isOpened())
    {
        return NULL;
    }

    // get next frame
    cv::Mat frame;
    *_video >> frame;

    // workaround: force opencv to grab a new frame
    if(forcedCapture)
        *_video >> frame;


    _lastFrame = new IPLImage(frame);

    return _lastFrame;
}

cv::VideoCapture *IPLVideoIO::camera()
{
    return _video;
}

void IPLVideoIO::release()
{
    if(_video)
        _video->release();
}
