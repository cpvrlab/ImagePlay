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

#ifndef IPLOPTICALFLOW_H
#define IPLOPTICALFLOW_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLKeyPoints.h"

#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/optflow.hpp"

/**
 * @brief The IPLOpticalFlow class
 */
class IPLSHARED_EXPORT IPLOpticalFlow : public IPLClonableProcess<IPLOpticalFlow>
{
public:
                            IPLOpticalFlow() : IPLClonableProcess() { init(); }
                            ~IPLOpticalFlow()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLData*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _image;
    IPLImage*               _previewX;
    IPLImage*               _previewY;
    IPLKeyPoints*           _keypoints;
    cv::Mat                 _image_prev, _image_next;
    std::vector<cv::Point>  _features_prev, _features_next;

private:
};

#endif // IPLOPTICALFLOW_H
