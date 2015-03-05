//#############################################################################
//  File:      IPProcessFactory.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPPROCESSFACTORY_H
#define IPPROCESSFACTORY_H

#include <QString>
#include <QVector>
#include <QMap>
#include <QStringList>

#include "IPLProcess.h"

//-----------------------------------------------------------------------------
//!IPProcessFactory creates and manages instances of IPLProcess
/*!
 * In order to be used, all processes need to be registered
 * using registerProcess
 */
class IPProcessFactory
{
public:
                        IPProcessFactory            ();
                        ~IPProcessFactory           ();
    void                registerProcess             (QString name, IPLProcess* getInstance);
    void                unregisterProcess           (QString name);
    IPLProcess*         getInstance                 (QString name);
    QStringList    getProcessNamesByCategory   (IPLProcess::IPLProcessCategory category);

private:
    QMap<QString, IPLProcess*>   _map;              //!< Contains all currently registered IPLProcesses by name
};

#endif // IPPROCESSFACTORY_H
