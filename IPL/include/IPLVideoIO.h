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

#ifndef IPLVideoIO_H
#define IPLVideoIO_H

#include "IPL_global.h"
#include "IPLImage.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief The IPLVideoIO class
 */
class IPLSHARED_EXPORT IPLVideoIO
{
public:
    static IPLImage*            grabFrame(std::string fileName, bool forcedCapture = false);
    static cv::VideoCapture*    camera();
    static void                 release();
private:
    static cv::VideoCapture*    _video;
    static IPLImage*            _lastFrame;
};

#endif // IPLVideoIO_H
