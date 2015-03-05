#ifndef IPLPROGRESSEVENTHANDLER_H
#define IPLPROGRESSEVENTHANDLER_H

#include "IPL_global.h"

/**
 * @brief The IPLProgressEventHandler class
 */
class IPLProgressEventHandler
{
public:
    IPLProgressEventHandler()
    {

    }
    virtual void updateProgress(int percent) = 0;
};

#endif // IPLPROGRESSEVENTHANDLER_H
