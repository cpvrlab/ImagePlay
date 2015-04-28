//#############################################################################
//
//  File:      IPLCamera.h
//  Author:    Kaspar Schmid
//  Date:      November 2014
//
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

#ifndef IPLCAMERA_H
#define IPLCAMERA_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLCameraIO.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


/**
 * @brief The IPLCamera class
 */
class IPLSHARED_EXPORT IPLCamera : public IPLClonableProcess<IPLCamera>
{
public:
                            IPLCamera() : IPLClonableProcess() { init(); }
                            ~IPLCamera()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    virtual bool            processInputData        (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData           (int outNr);
protected:
    IPLImage*               _result;
    bool                    _continuous;
};

#endif // IPLCAMERA_H
