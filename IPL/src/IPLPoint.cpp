#include "IPLPoint.h"

IPLPoint::IPLPoint() : IPLData(IPLData::POINT)
{
}

IPLPoint::IPLPoint(double x, double y) : IPLPoint()
{
    _x = x;
    _y = y;
}

IPLPoint::IPLPoint(const IPLPoint &other)
{
    _x = other._x;
    _y = other._y;
}

IPLPoint &IPLPoint::operator=(const IPLPoint &other)
{
    _x = other._x;
    _y = other._y;
    return *this;
}

IPLPoint::~IPLPoint()
{

}

std::string IPLPoint::toString(int x, int y)
{
    std::stringstream s;
    s << "X: " << x << " Y: " << y;

    return s.str();
}
