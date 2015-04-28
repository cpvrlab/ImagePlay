//#############################################################################
//
//  File:      IPL_pluininterface.h
//  Author:    Kaspar Schmid
//  Date:      November 2014
//
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
