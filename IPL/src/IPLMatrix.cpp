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

#include "IPLMatrix.h"


IPLMatrix::IPLMatrix(int height, int width) : IPLData(IPL_MATRIX)
{
    _height = height;
    _width = width;
}

IPLMatrix::IPLMatrix(int height, int width, ipl_basetype array[]) : IPLData(IPL_MATRIX)
{
    _height = height;
    _width = width;

    _data.insert(_data.begin(), array, array + height * width);
}

IPLMatrix::IPLMatrix(const IPLMatrix &other) : IPLData(IPL_MATRIX)
{
    _data = other._data;
    _height = other._height;
    _width = other._width;
}

IPLMatrix::~IPLMatrix()
{

}

ipl_basetype IPLMatrix::get(int x, int y)
{
    return _data[x + y * _width];
}

ipl_basetype IPLMatrix::get(int i)
{
    return _data[i];
}

void IPLMatrix::set(int x, int y, ipl_basetype value)
{
    _data[x + y * _width] = value;
}

void IPLMatrix::set(int i, ipl_basetype value)
{
    _data[i] = value;
}
