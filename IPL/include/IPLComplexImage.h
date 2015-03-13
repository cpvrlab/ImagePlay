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
class IPLSHARED_EXPORT IPLComplexImage : public IPLImage
{
public:
    IPLComplexImage                 ();
    IPLComplexImage                 (const IPLImage& image);
    IPLComplexImage                 (int width, int height);
    ~IPLComplexImage                ();

    ipl_basetype&   real            (int x, int y);
    ipl_basetype&   imag            (int x, int y);

    void            flip            (void);
    Complex*        getRow          (int y);
    bool            FFT             ();
    bool            IFFT            ();

    Complex**       _plane;

    // auxiliary class methods
    static int      nextPowerOf2    (int x);
    static void     Twiddle         (Complex* x, int N);
    static void     iTwiddle        (Complex* x, int N);
    static void     lineFFT         (Complex* x, Complex* twiddle, int N);
};

#endif // IPLCOMPLEXIMAGE_H
