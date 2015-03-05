#include "IPLMatrix.h"


IPLMatrix::IPLMatrix(int height, int width) : IPLData(IPLData::MATRIX)
{
    _height = height;
    _width = width;
}

IPLMatrix::IPLMatrix(int height, int width, ipl_basetype array[]) : IPLData(IPLData::MATRIX)
{
    _height = height;
    _width = width;

    _data.insert(_data.begin(), array, array + height * width);
}

IPLMatrix::IPLMatrix(const IPLMatrix &other) : IPLData(IPLData::MATRIX)
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
