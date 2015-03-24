#ifndef IPLCOMPLEXIMAGE_H
#define IPLCOMPLEXIMAGE_H

#include "IPL_global.h"

#include "IPLImage.h"

#include <complex>
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

    void            newPlane        ();
    unsigned char*  rgb32           (void);
    int width(void)                 { return _width; }
    int height(void)                { return _height; }

    Complex&        c               (int x, int y);
    ipl_basetype    real            (int x, int y);
    ipl_basetype    imag            (int x, int y);
    ipl_basetype    maxReal         ();
    ipl_basetype    minReal         ();

    void            flip            (void);
    bool            FFT             ();
    bool            IFFT            ();

    // auxiliary class methods
    static int      nextPowerOf2    (int x);
    static void     Twiddle         (Complex* x, int N);
    static void     iTwiddle        (Complex* x, int N);
    static void     lineFFT         (Complex* x, Complex* twiddle, int N);
private:
    int             _height;
    int             _width;
    Complex**       _plane;
    unsigned char*  _rgb32;
};

#endif // IPLCOMPLEXIMAGE_H
