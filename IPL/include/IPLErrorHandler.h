#ifndef IPLERRORHANDLER_H
#define IPLERRORHANDLER_H

#include "IPL_global.h"

/**
 * @brief The IPLErrorHandler class
 */
class IPLErrorHandler
{
public:
    IPLErrorHandler()
    {

    }
    virtual void error() = 0;
};

#endif // IPLERRORHANDLER_H
