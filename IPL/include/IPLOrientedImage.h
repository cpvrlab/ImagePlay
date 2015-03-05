#ifndef IPLORIENTEDIMAGE_H
#define IPLORIENTEDIMAGE_H

#include "IPL_global.h"

#include "IPLImage.h"

/**
 * @brief The IPLComplexImage class
 */
class IPLSHARED_EXPORT IPLOrientedImage : public IPLImage
{
public:
    IPLOrientedImage();
    IPLOrientedImage( const IPLImage& image );
    IPLOrientedImage( int width, int height );
    ~IPLOrientedImage();

    ipl_basetype& magnitude(int x, int y);
    ipl_basetype& phase(int x, int y);
};

#endif // IPLORIENTEDIMAGE_H
