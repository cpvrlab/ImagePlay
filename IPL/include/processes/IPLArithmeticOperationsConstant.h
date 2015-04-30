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

#ifndef IPLARITHMETICOPERATIONSCONSTANT_H
#define IPLARITHMETICOPERATIONSCONSTANT_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/**
 * @brief The IPLArithmeticOperationsConstant class
 */
class IPLSHARED_EXPORT IPLArithmeticOperationsConstant : public IPLClonableProcess<IPLArithmeticOperationsConstant>
{
public:
    IPLArithmeticOperationsConstant() : IPLClonableProcess() { init(); }
    ~IPLArithmeticOperationsConstant()  { destroy(); }
    void                init();
    void                destroy();
    virtual bool        processInputData                        (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*   getResultData                           (int outNr);

protected:
    int                 _operation;
    float               _constant;
    IPLImage*           _result;
};


#endif // IPLARITHMETICOPERATIONSCONSTANT_H
