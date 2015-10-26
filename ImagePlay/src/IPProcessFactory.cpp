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

#include "IPProcessFactory.h"

IPProcessFactory::IPProcessFactory()
{
}

IPProcessFactory::~IPProcessFactory()
{
    // delete all template instances
/*    for(QMap<QString, IPLProcess*>::iterator it = _map.begin(); it != _map.end(); it++)
    {
        IPLProcess* p = it.value();
        delete p;
    }*/
}

void IPProcessFactory::registerProcess(QString name, IPLProcess *process)
{
    qDebug() << "registerProcess: " << name << " -> " << process;
    _map.insert(name, process);
}

void IPProcessFactory::unregisterProcess(QString name)
{
    qDebug() << "unregisterProcess: " << name;
    if(_map.contains(name))
    {
        // delete instance and remove from map
        delete _map.value(name);
        _map.remove(name);
    }
}

IPLProcess* IPProcessFactory::getInstance(QString name)
{
    qDebug() << "getInstance: " << name;
    return NULL;
    if(_map.contains(name))
    {
        // return a fresh copy of the template process
        IPLProcess* process = _map.value(name)->clone();
        process->properties()->clear();
        process->inputs()->clear();
        process->outputs()->clear();
        process->init();
        return process;
    }
    else
    {
        return NULL;
    }
}

QStringList IPProcessFactory::getProcessNamesByCategory(IPLProcess::IPLProcessCategory category)
{
    QStringList filteredList;
    for (auto it = _map.begin(); it != _map.end(); ++it)
    {
        if(it.value()->category() == category)
        {
            filteredList.push_back(it.key());
        }
    }
    return filteredList;
}
