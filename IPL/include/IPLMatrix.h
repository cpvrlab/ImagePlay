#ifndef IPLMATRIX_H
#define IPLMATRIX_H

#include "IPL_global.h"

#include "IPLData.h"

#include <vector>

/**
 * @brief The IPLImage class
 */
class IPLSHARED_EXPORT IPLMatrix : public IPLData
{
public:
                    IPLMatrix       (int height, int width);
                    IPLMatrix       (int height, int width, ipl_basetype array[]);
                    IPLMatrix       (const IPLMatrix& other);
                    ~IPLMatrix      ();

    ipl_basetype    get             (int x, int y);
    ipl_basetype    get             (int i);
    void            set             (int x, int y, ipl_basetype value);
    void            set             (int i, ipl_basetype value);
    int             height          ()                                                  { return _height; }
    int             width           ()                                                  { return _width; }
    int             size            ()                                                  { return _height * _width; }

protected:
    int                             _width;
    int                             _height;
    std::vector<ipl_basetype>       _data;
};

#endif // IPLMATRIX_H
