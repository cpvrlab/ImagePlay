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

#ifndef IPLRANDOMPOINT_H
#define IPLRANDOMPOINT_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLPoint.h"

#include <string>
#include <time.h>

/**
 * @brief The IPLRandomPoint class
 */
class IPLSHARED_EXPORT IPLRandomPoint : public IPLClonableProcess<IPLRandomPoint>
{
public:
    IPLRandomPoint() : IPLClonableProcess() {}
    ~IPLRandomPoint()  { destroy(); }


    void init();
    void destroy();

    virtual bool processInputData( IPLData* data, int inNr, bool useOpenCV );
    virtual IPLData* getResultData( int outNr );

protected:
    IPLImage*   _image;
    IPLPoint*   _point;
};

#endif // IPLRANDOMPOINT_H
