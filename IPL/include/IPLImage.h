#ifndef IPLIMAGE_H
#define IPLIMAGE_H

#include "IPL_global.h"

#include "IPLData.h"
#include "IPLImagePlane.h"
#include "IPLColor.h"

#include <vector>
#include <sstream>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

/**
 * @brief The IPLImage class
 */
class IPLSHARED_EXPORT IPLImage : public IPLData
{
public:
    IPLImage();
    IPLImage( const IPLImage& image );
    IPLImage( IPLData::IPLDataType _type, int width, int height );
    IPLImage( cv::Mat& cvMat );
    ~IPLImage();

    int getNumberOfPlanes( void )   { return _nrOfPlanes; }
    int width(void)                 { return _width; }
    int height(void)                { return _height; }

    unsigned char* rgb32 (void);
    IPLImagePlane* plane( int planeNr );
    void fillColor( ipl_basetype color );

    std::string                 toString(int x, int y);

    cv::Mat                     toCvMat();

protected:
    int                         _width;
    int                         _height;
    int                         _nrOfPlanes;
    unsigned char*              _rgb32;
    static int                  _instanceCount;
    std::vector<IPLImagePlane*> _planes;
};

#endif // IPLIMAGE_H
