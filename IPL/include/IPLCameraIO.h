#ifndef IPLCAMERAIO_H
#define IPLCAMERAIO_H

#include "IPL_global.h"
#include "IPLImage.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief The IPLCameraIO class
 */
class IPLSHARED_EXPORT IPLCameraIO
{
public:
    static IPLImage*            grabFrame(bool forcedCapture = false);
    static cv::VideoCapture*    camera();
    static void                 release();
private:
    static cv::VideoCapture*    _camera;
    static IPLImage*            _lastFrame;
};

#endif // IPLCAMERAIO_H
