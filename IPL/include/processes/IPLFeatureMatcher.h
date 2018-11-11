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

#ifndef IPLFEATUREMATCHER_H
#define IPLFEATUREMATCHER_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLKeyPoints.h"

#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/xfeatures2d.hpp"

/**
 * @brief The IPLFeatureMatcher class
 */
class IPLSHARED_EXPORT IPLFeatureMatcher : public IPLClonableProcess<IPLFeatureMatcher>
{
public:
                            IPLFeatureMatcher() : IPLClonableProcess() { init(); }
                            ~IPLFeatureMatcher()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLData*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _image1;
    IPLImage*               _image2;
    IPLImage*               _preview;
    IPLKeyPoints*           _keypoints1;
    IPLKeyPoints*           _keypoints2;

private:
};

#endif // IPLFEATUREMATCHER_H
