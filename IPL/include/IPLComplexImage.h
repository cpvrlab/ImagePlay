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

#ifndef IPLCOMPLEXIMAGE_H
#define IPLCOMPLEXIMAGE_H

#include "IPL_global.h"

#include "IPLImage.h"

#include <complex>
#include <mutex>

using std::complex;
typedef std::complex<double> Complex;

/**
 * @brief The IPLComplexImage class
 */
class IPLSHARED_EXPORT IPLComplexImage : public IPLData
{
public:
                    IPLComplexImage                 ();
                    IPLComplexImage                 (const IPLComplexImage& other);
                    IPLComplexImage                 (int width, int height);
                    ~IPLComplexImage                ();

    void            newPlane                        ();
    unsigned char*  rgb32                           (int mode = 0);
    int             width                           (void)                  { return _width; }
    int             height                          (void)                  { return _height; }

    Complex&        c                               (int x, int y);
    ipl_basetype    real                            (int x, int y);
    ipl_basetype    imag                            (int x, int y);
    ipl_basetype    maxReal                         ();
    ipl_basetype    minReal                         ();

    void            flip                            (void);
    bool            FFT                             ();
    bool            IFFT                            ();

    // auxiliary class methods
    static int      nextPowerOf2                    (int x);
    static void     Twiddle                         (Complex* x, int N);
    static void     iTwiddle                        (Complex* x, int N);
    static void     lineFFT                         (Complex* x, Complex* twiddle, int N);
private:
    int                 _height;
    int                 _width;
    Complex**           _plane;
    std::vector<uchar>  _rgb32;
    std::mutex          _mutex;
};

#endif // IPLCOMPLEXIMAGE_H
