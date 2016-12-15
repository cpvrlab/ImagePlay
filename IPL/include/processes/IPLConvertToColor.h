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

#ifndef IPLCONVERTTOCOLOR_H
#define IPLCONVERTTOCOLOR_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/*!
 * Converts a 1-channel grayscale or b/w image to 3-channel RGB
 */
class IPLSHARED_EXPORT IPLConvertToColor : public IPLClonableProcess<IPLConvertToColor>
{
public:
    IPLConvertToColor() : IPLClonableProcess() { init(); }
    ~IPLConvertToColor()  { destroy(); }

    virtual void            init                        ();
    virtual void            destroy();
    virtual bool            processInputData            (IPLData* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData               (int outNr);

protected:
    int                     _operation;
    IPLImage*               _inputA;
    IPLImage*               _inputB;
    IPLImage*               _result;
};


#endif // IPLCONVERTTOCOLOR_H
