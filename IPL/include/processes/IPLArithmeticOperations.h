//#############################################################################
//
//  File:      IPLArithmeticOperations.h
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

#ifndef IPLARITHMETICOPERATIONS_H
#define IPLARITHMETICOPERATIONS_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/*!
 *
 */
class IPLSHARED_EXPORT IPLArithmeticOperations : public IPLClonableProcess<IPLArithmeticOperations>
{
public:
                        IPLArithmeticOperations     () : IPLClonableProcess() { init(); }
                        ~IPLArithmeticOperations    () { destroy(); }
    void                init                        ();
    void                destroy();
    bool                processInputData            (IPLImage* data, int inNr, bool useOpenCV);
    IPLImage*           getResultData               (int outNr);

private:
    int                 _operation;
    IPLImage*           _inputA;
    IPLImage*           _inputB;
    IPLImage*           _result;
};


#endif // IPLARITHMETICOPERATIONS_H
