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

#ifndef IPLLABELBLOBS_H
#define IPLLABELBLOBS_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>
#include <deque>

/**
 * @brief The IPLLabelBlobs class
 */
class IPLSHARED_EXPORT IPLLabelBlobs : public IPLClonableProcess<IPLLabelBlobs>
{
public:
                            IPLLabelBlobs() : IPLClonableProcess() { init(); }
                            ~IPLLabelBlobs()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
private:
    void                    labelBlob(IPLImagePlane *inPlane, IPLImagePlane *outPlane, int x, int y, float label);

    struct Pixel {
        Pixel( int _x, int _y ){ x=_x; y=_y; };
        int x;
        int y;
    };

    typedef std::deque<IPLLabelBlobs::Pixel> Queue;
};


#endif // IPLLABELBLOBS_H
