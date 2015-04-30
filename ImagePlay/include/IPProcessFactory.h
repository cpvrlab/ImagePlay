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
    QStringList         getProcessNamesByCategory   (IPLProcess::IPLProcessCategory category);

private:
    QMap<QString, IPLProcess*>   _map;              //!< Contains all currently registered IPLProcesses by name
};

#endif // IPPROCESSFACTORY_H
