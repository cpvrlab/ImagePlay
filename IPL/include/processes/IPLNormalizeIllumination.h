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

#ifndef IPLNORMALIZEILLUMINATION_H
#define IPLNORMALIZEILLUMINATION_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLNormalizeIllumination class
 */
class IPLSHARED_EXPORT IPLNormalizeIllumination : public IPLClonableProcess<IPLNormalizeIllumination>
{
public:
                            IPLNormalizeIllumination() : IPLClonableProcess() { init(); }
                            ~IPLNormalizeIllumination()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
    IPLImage*               _illumination;
    int                     _cycles;
    double                  _factor;
private:
    void normalizePlane(IPLImagePlane *plane);
    void HLStoRGB(double H, double L, double S, double &R, double &G, double &B);
    double _RGB(double q1, double q2, double H);
    void RGBtoHLS(double R, double G, double B, double &H, double &L, double &S);
    double max_of(double x1, double x2, double x3);
    double min_of(double x1, double x2, double x3);
};

#endif // IPLNORMALIZEILLUMINATION_H
