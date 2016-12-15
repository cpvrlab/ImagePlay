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

#include "IPPluginManager.h"

IPPluginManager::IPPluginManager()
{

}

void IPPluginManager::loadPlugins(QString pluginPath, IPProcessFactory* factory)
{
    // plugin loading doesn't work in debug mode
#ifdef QT_DEBUG
    return;
#endif

#if defined(_WIN32)
    static const auto pluginFilter = QStringList() << "*.dll";
#else
    static const auto pluginFilter = QStringList() << "*.so";
#endif

    qDebug() << "IPPluginManager::loadPlugins: " << pluginPath;

    _pluginPath = pluginPath;
    _factory = factory;
    _loadedPlugins.clear();

    QDir pluginsDir(_pluginPath);

    // watch the original dll for changes
    // _pluginFileSystemWatcher->addPath(pluginPath());

    QDateTime now = QDateTime::currentDateTime();
    _pluginTmpPath = _pluginPath + "/tmp/";
    QDir tmpPluginsDir(_pluginTmpPath);

    // delete old tmp directory
    if(!removeDir(_pluginTmpPath))
    {
        qWarning() << "Could not remove old plugin dir: " << _pluginTmpPath;
        return;
    }

    // create new directory, copy dlls
    if(!tmpPluginsDir.exists())
        tmpPluginsDir.mkpath(".");

    pluginsDir.setNameFilters(pluginFilter);
    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QFile file(pluginsDir.filePath(fileName));
        file.copy(tmpPluginsDir.filePath(fileName));
    }

    if(tmpPluginsDir.exists())
    {
        tmpPluginsDir.setNameFilters(pluginFilter);

        foreach (QString fileName, tmpPluginsDir.entryList(QDir::Files))
        {
            QString pluginFilePath = _pluginTmpPath + "/" + fileName;
            pugg::Kernel* kernel = new pugg::Kernel;
            _kernels.push_back(kernel);
            kernel->add_server(IPLProcess::server_name(), IPLProcess::version);
            kernel->load_plugin(pluginFilePath.toStdString());

            // we can load all drivers from a specific server
            _drivers = kernel->get_all_drivers<IPLProcessDriver>(IPLProcess::server_name());

            if(_drivers.size() == 0)
            {
                QString msg("Plugin IPL version does not match ImagePlay IPL API version %2.\nCan't load %3.");
                QMessageBox::warning(NULL, "Plugin Error", msg.arg(IPL_VERSION).arg(fileName));
                continue;
            }

            for (std::vector<IPLProcessDriver*>::iterator iter = _drivers.begin(); iter != _drivers.end(); ++iter) {
                IPLProcessDriver& driver = *(*iter);

                QString className = QString::fromStdString(driver.className());
                QString author = QString::fromStdString(driver.author());
                int version = driver.version();

                IPLProcess* pluginInstance = driver.create();
                _factory->registerProcess(className, pluginInstance);
                _loadedPlugins.push_back(className);
            }
        }
    }
}

void IPPluginManager::unloadPlugins()
{
    // unregister process
    for(int i=0; i<_loadedPlugins.count(); i++)
    {
        QString plugin = _loadedPlugins.at(i);
        _factory->unregisterProcess(plugin);
    }
    _loadedPlugins.clear();

    // unload DLL
    for(int i=0; i<_kernels.count(); i++)
    {
        pugg::Kernel* kernel = _kernels.at(i);
        kernel->clear();
    }
    _kernels.clear();
    _drivers.clear();

    // delete old tmp directory
    removeDir(_pluginTmpPath);
}

void IPPluginManager::on_pluginDirectoryChanged(const QString & path)
{
    // problem: when compiling a plugin, the file is changed multiple times,
    // we don't know when the final edit has hapened...
    // it is not possible to watch the DLL only, because when it is deleted by
    // the compiler, the filesystem watcher stops automatically.
    // hmmmm.

    //qDebug() << "Changed: " << path;
    /*QDir pluginDirectory(path);

    // only reload if files have been added
    //if(pluginDirectory.count() > _pluginFileSytemLastCount)
    //    reloadPlugins();

    delete _pluginFileSystemTimer;
    _pluginFileSystemTimer = new QTimer(this);
    _pluginFileSystemTimer->setSingleShot(true);
    _pluginFileSystemTimer->start(3000);
    connect(_pluginFileSystemTimer, &QTimer::timeout, this, &MainWindow::reloadPlugins);
    qDebug() << "start";

    _pluginFileSytemLastCount = pluginDirectory.count();*/
    //qDebug() << _pluginFileSytemLastCount;
}



bool IPPluginManager::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}
