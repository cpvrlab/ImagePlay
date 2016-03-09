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

#ifndef IPLCANNY_H
#define IPLCANNY_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"
#include "IPLOrientedImage.h"

#include <string>
#include <deque>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief The IPLCanny class
 */
class IPLSHARED_EXPORT IPLCanny : public IPLClonableProcess<IPLCanny>
{
public:
                            IPLCanny() : IPLClonableProcess() { init(); }
                            ~IPLCanny()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLData*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
    IPLImage*               _binaryImage;
    IPLOrientedImage*       _orientedImage;

private:
    double gauss(double x, double ssq);
    double dGauss(double x, double, double ssq);
    double Norm(double x, double y);
    void trace(int x, int y, double lowThreshold, IPLOrientedImage *dI, IPLImagePlane *image);
    void thinning(IPLOrientedImage *dI, IPLImagePlane *image, IPLImagePlane *newplane);

    class Pixel {
        public: Pixel( int _x, int _y ){ x=_x; y=_y; }
        int x;
        int y;
    };
    typedef std::deque<Pixel> Queue;
};

#endif // IPLCANNY_H
