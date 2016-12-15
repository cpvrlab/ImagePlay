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

#ifndef IPPLUGINMANAGER_H
#define IPPLUGINMANAGER_H

#include <QString>
#include <QList>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

#include "IPLProcess.h"
#include "IPProcessFactory.h"
#include <pugg/Kernel.h>

class IPPluginManager
{
public:
    IPPluginManager();

    void                           loadPlugins(QString pluginPath, IPProcessFactory* factory);
    void                           unloadPlugins();
    void                           reloadPlugins();
    QList<QString>*                loadedPlugins() { return &_loadedPlugins; }
    std::vector<IPLProcessDriver*>* loadedDrivers() { return &_drivers; }
    bool                           removeDir(const QString &dirName);

private slots:
    void                           on_pluginDirectoryChanged       (const QString & path);

private:
    QList<QString>                 _loadedPlugins;
    QString                        _pluginPath;
    QString                        _pluginTmpPath;
    QFileSystemWatcher*            _pluginFileSystemWatcher;
    int                            _pluginFileSytemLastCount;
    QTimer*                        _pluginFileSystemTimer;
    IPProcessFactory*              _factory;
    QList<pugg::Kernel*>           _kernels;
    std::vector<IPLProcessDriver*> _drivers;
};

#endif // IPPLUGINMANAGER_H
