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

#ifndef IPLWarpAffine_H
#define IPLWarpAffine_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

/**
 * @brief The IPLWarpAffine class
 */
class IPLSHARED_EXPORT IPLWarpAffine : public IPLClonableProcess<IPLWarpAffine>
{
public:
                            IPLWarpAffine() : IPLClonableProcess() { init(); }
                            ~IPLWarpAffine()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLData*, int index, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
    IPLImage*               _inputA;
    IPLImage*               _inputB;
    IPLImage*               _previewA;
    IPLImage*               _previewB;
};

#endif // IPLWarpAffine_H
