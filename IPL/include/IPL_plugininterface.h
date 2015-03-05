#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include "IPLImage.h"
#include "IPLProcess.h"

class PluginInterface
{
public:
    virtual ~PluginInterface()  {}
    virtual QString name() = 0;
    virtual QString version() = 0;
    virtual QString author() = 0;
    virtual IPLProcess* getProcess() = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "ch.bfh.cpvrlab.ImagePlay.PluginInterface/1.0")

#endif // PLUGININTERFACE_H
