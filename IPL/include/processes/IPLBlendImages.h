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

#ifndef IPLCOMBINEIMAGES_H
#define IPLCOMBINEIMAGES_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>
#include <cmath>

/**
 * @brief The IPLBlendImages class
 */
class IPLSHARED_EXPORT IPLBlendImages : public IPLClonableProcess<IPLBlendImages>
{
public:
    IPLBlendImages() : IPLClonableProcess() { init(); }
    ~IPLBlendImages()  { destroy(); }

    void                    init();
    virtual void            destroy();

    virtual bool            processInputData    (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData       (int outNr);

protected:
    IPLImage*               _result;
    IPLImage*               _inputA;
    IPLImage*               _inputB;
    int                     _operation;
    double                  _factorA;
    double                  _factorB;
};

#endif // IPLCOMBINEIMAGES_H
