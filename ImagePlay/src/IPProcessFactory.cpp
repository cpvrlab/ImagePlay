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
    _map.insert(name, process);
}

void IPProcessFactory::unregisterProcess(QString name)
{
    if(_map.contains(name))
    {
        // delete instance and remove from map
        delete _map.value(name);
        _map.remove(name);
    }
}

IPLProcess* IPProcessFactory::getInstance(QString name)
{
    if(_map.contains(name))
    {
        // return a fresh copy of the template process
        IPLProcess* process = _map.value(name)->clone();
        process->properties()->clear();
        process->inputs()->clear();
        process->outputs()->clear();
        //process->addProcessPropertyString("title", "Title", "", IPL_STRING, process->title());
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
