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

#ifndef IPLIFFT_H
#define IPLIFFT_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLComplexImage.h"

#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

/**
 * @brief The IPLIFFT class
 */
class IPLSHARED_EXPORT IPLIFFT : public IPLClonableProcess<IPLIFFT>
{
public:
                            IPLIFFT() : IPLClonableProcess() { init(); }
                            ~IPLIFFT()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLData*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLComplexImage*        _complexImage;
    IPLImage*               _result;
};

#endif // IPLIFFT_H
