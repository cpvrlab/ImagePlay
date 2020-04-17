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

#ifndef IPLIMAGE_H
#define IPLIMAGE_H

#include "IPL_global.h"

#include "IPLData.h"
#include "IPLImagePlane.h"
#include "IPLColor.h"

#include <vector>
#include <sstream>
#include <stdexcept>


#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

/**
 * @brief The IPLImage class
 */
class IPLSHARED_EXPORT IPLImage : public IPLData
{
public:
    IPLImage();
    IPLImage( const IPLImage& image );
    IPLImage( IPLDataType _type, int width, int height );
    IPLImage( cv::Mat& cvMat );
    ~IPLImage();

    int getNumberOfPlanes( void )   { return _nrOfPlanes; }
    int width(void)                 { return _width; }
    int height(void)                { return _height; }

    uchar* rgb32 (void);
    static void rgb32CleanupHandler(void *info);
    IPLImagePlane* plane(int planeNr);
    void fillColor( ipl_basetype color );

    std::string                 toString(int x, int y);

    cv::Mat                     toCvMat();

protected:
    int                         _width;
    int                         _height;
    int                         _nrOfPlanes;
    std::vector<uchar>          _rgb32;
    static int                  _instanceCount;
    std::vector<IPLImagePlane*> _planes;
};

#endif // IPLIMAGE_H
