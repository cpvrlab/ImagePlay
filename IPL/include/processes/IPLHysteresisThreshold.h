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

#ifndef IPLHysteresisThreshold_H
#define IPLHysteresisThreshold_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <deque>

/**
 * @brief The IPLHysteresisThreshold class
 */
class IPLSHARED_EXPORT IPLHysteresisThreshold : public IPLClonableProcess<IPLHysteresisThreshold>
{
public:
                            IPLHysteresisThreshold() : IPLClonableProcess() { init(); }
                            ~IPLHysteresisThreshold()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;

    class Pixel {
        public: Pixel( int _x, int _y ){ x=_x; y=_y; }
        int x;
        int y;
    };
    typedef std::deque<Pixel> Queue;

private:
    int trace(int x, int y, IPLImagePlane *plane, IPLImagePlane *image, float lowThreshold);
};

#endif // IPLHysteresisThreshold_H
