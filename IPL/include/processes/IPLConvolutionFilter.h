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

#ifndef IPLCONVOLUTIONFILTER_H
#define IPLCONVOLUTIONFILTER_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/**
 * @brief The IPLConvolutionFilter class
 */
class IPLSHARED_EXPORT IPLConvolutionFilter : public IPLClonableProcess<IPLConvolutionFilter>
{
public:
    IPLConvolutionFilter() : IPLClonableProcess() { init(); }
    ~IPLConvolutionFilter()  { destroy(); }

    void                    init                    ();
    virtual void        destroy();

    virtual bool            processInputData        (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData           (int outNr);

protected:
    IPLImage*               _result;
    std::vector<int>        _kernel;
    float                   _offset;
    int                     _divisor;
    int                     _borders;
    bool                    _normalize;
};

#endif // IPLCONVOLUTIONFILTER_H
