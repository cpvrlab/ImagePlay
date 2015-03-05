#ifndef IPLPROPERTYCHANGEDEVENTHANDLER_H
#define IPLPROPERTYCHANGEDEVENTHANDLER_H

#include "IPL_global.h"

class IPLProcess;

/**
 * @brief The IPLPropertyChangedEventHandler class
 */
class IPLPropertyChangedEventHandler
{
public:
    IPLPropertyChangedEventHandler()
    {

    }
    virtual void propertyChanged(IPLProcess*) = 0;
};

#endif // IPLPROPERTYCHANGEDEVENTHANDLER_H
