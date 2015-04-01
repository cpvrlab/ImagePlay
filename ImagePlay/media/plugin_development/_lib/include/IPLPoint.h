#ifndef IPLPOINT_H
#define IPLPOINT_H

#include "IPL_global.h"

#include "IPLData.h"

#include <sstream>

/**
 * @brief The IPLPoint class
 */
class IPLSHARED_EXPORT IPLPoint : public IPLData
{
public:
    IPLPoint();
    IPLPoint(double x, double y);
    IPLPoint(const IPLPoint& other);
    IPLPoint& operator=(const IPLPoint& other);
    ~IPLPoint();

    void setX(double value)       { _x = value; }
    void setY(double value)       { _y = value; }

    double x() const  { return _x; }
    double y() const  { return _y; }

    std::string toString(int x, int y);
private:
    double _x;
    double _y;
};


#endif // IPLPOINT_H
