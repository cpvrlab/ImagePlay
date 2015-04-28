//#############################################################################
//
//  File:      IPLGaussianLowPass.h
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

#ifndef IPLGAUSSIANLOWPASS_H
#define IPLGAUSSIANLOWPASS_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief The IPLGaussianLowPass class
 */
class IPLSHARED_EXPORT IPLGaussianLowPass : public IPLClonableProcess<IPLGaussianLowPass>
{
public:
    IPLGaussianLowPass() : IPLClonableProcess() { init(); }
    ~IPLGaussianLowPass()  { destroy(); }

    void                    init                    ();
    virtual void            destroy();

    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

    static void             gauss                   (IPLImage* image, IPLImage* result, IPLData* kernel, int height, int window, double sigma, int width, int N);
protected:
    IPLImage*               _result;
    IPLData*                _kernel;
};

#endif // IPLGAUSSIANLOWPASS_H
