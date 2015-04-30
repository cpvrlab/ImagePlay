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
