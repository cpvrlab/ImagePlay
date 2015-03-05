#ifndef IPLDATA_H
#define IPLDATA_H

#include "IPL_global.h"

#include <string>

class IPLImage;
class IPLPoint;
class IPLMatrix;

class IPLSHARED_EXPORT IPLData
{
public:
    enum                IPLDataType     { IMAGE_BW, IMAGE_GRAYSCALE, IMAGE_COLOR, IMAGE_ORIENTED, IMAGE_COMPLEX, POINT, MATRIX, SHAPES, UNDEFINED };

    IPLData         ()                      { _type = UNDEFINED; }
    IPLData        (IPLDataType type)      { _type = type; }
    virtual             ~IPLData()                              {}
    IPLDataType         type            (void)                  { return _type; }

    IPLImage*           toImage();
    IPLPoint*           toPoint();
    IPLMatrix*          toMatrix();

protected:
    IPLDataType         _type;
};

#endif // IPLDATA_H
