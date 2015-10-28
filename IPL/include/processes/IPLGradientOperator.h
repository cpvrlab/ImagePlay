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

#ifndef IPLGRADIENTOPERATOR_H
#define IPLGRADIENTOPERATOR_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLOrientedImage.h"

#include <string>

class IPLOrientedImage;

/**
 * @brief The IPLFlipImage class
 */
class IPLSHARED_EXPORT IPLGradientOperator : public IPLClonableProcess<IPLGradientOperator>
{
public:
    IPLGradientOperator() : IPLClonableProcess() { init(); }
    ~IPLGradientOperator()  { destroy(); }

    void                    init();
    virtual void        destroy();

    virtual bool            processInputData            (IPLImage* data, int inNr, bool) override;
    virtual IPLImage*       getResultData               (int outNr);

protected:
    IPLOrientedImage*   _result;
    IPLImage*           _magnitude;
    IPLImage*           _phase;

private:
    bool fastGradient(IPLImage* data);
    bool roberts(IPLImage* data);
    bool sobel(IPLImage* data);
    bool cubicSpline(IPLImage* data);
};

#endif // IPLGRADIENTOPERATOR_H
