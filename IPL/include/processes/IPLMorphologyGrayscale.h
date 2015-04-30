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

#ifndef IPLMORPHOLOGYGRAYSCALE_H
#define IPLMORPHOLOGYGRAYSCALE_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/**
 * @brief The IPLMorphologyGrayscale class
 */
class IPLSHARED_EXPORT IPLMorphologyGrayscale : public IPLClonableProcess<IPLMorphologyGrayscale>
{
public:
    IPLMorphologyGrayscale() : IPLClonableProcess() { init(); }
    ~IPLMorphologyGrayscale()  { destroy(); }

    void init();
    virtual void        destroy();

    virtual bool processInputData( IPLImage* data, int inNr, bool useOpenCV );
    virtual IPLImage* getResultData( int outNr );

    void dilate(IPLImagePlane* workingPlane, IPLImagePlane* resultPlane);

    uchar max_value(std::vector<uchar> a);

protected:
    IPLImage*           _result;
    std::vector<int>    _kernel;
    int                 _operation;
    int                 _iterations;
};

#endif // IPLMORPHOLOGYGRAYSCALE_H
