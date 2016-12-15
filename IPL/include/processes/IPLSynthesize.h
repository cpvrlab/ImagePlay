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

#ifndef IPLSYNTHESIZE_H
#define IPLSYNTHESIZE_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLSynthesize class
 */
class IPLSHARED_EXPORT IPLSynthesize : public IPLClonableProcess<IPLSynthesize>
{
public:
    IPLSynthesize() : IPLClonableProcess() { init(); }
    ~IPLSynthesize()  { destroy(); }

    void init();
    virtual void        destroy();
    virtual bool processInputData(IPLData*, int inNr, bool useOpenCV );
    virtual IPLImage* getResultData( int outNr );
protected:
    IPLImage*   _result;
    int         _type;
    int         _width;
    int         _height;
    float       _amplitude;
    float       _offset;
    int         _wavelength;
    int         _direction;
    int         _decay;
};


#endif // IPLSYNTHESIZE_H
