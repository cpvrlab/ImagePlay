#ifndef IPLCOMPLEXIMAGE_H
#define IPLCOMPLEXIMAGE_H

#include "IPL_global.h"

#include "IPLImage.h"

/**
 * @brief The IPLComplexImage class
 */
class IPLSHARED_EXPORT IPLComplexImage : public IPLImage
{
public:
    IPLComplexImage();
    IPLComplexImage( const IPLImage& image );
    IPLComplexImage( int width, int height );
    ~IPLComplexImage();

    ipl_basetype real(int x, int y);
    ipl_basetype imag(int x, int y);
};

#endif // IPLCOMPLEXIMAGE_H
