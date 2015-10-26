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
#if defined(_WIN32)
    static const auto pluginFilter = QStringList() << "*.dll";
#else
    static const auto pluginFilter = QStringList() << "*.so";
#endif

    _pluginPath = pluginPath;
    _factory = factory;
    _loadedPlugins.clear();

    QDir pluginsDir(_pluginPath);

    // watch the original dll for changes
    // _pluginFileSystemWatcher->addPath(pluginPath());

    QDateTime now = QDateTime::currentDateTime();
    _pluginTmpPath = _pluginPath + "/tmp/" + now.toString("yyyyMMddhhmmss");
    QDir tmpPluginsDir(_pluginTmpPath);

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
            QString pluginFilePath = _pluginPath + "/" + fileName;
            pugg::Kernel kernel;
            kernel.add_server(IPLProcess::server_name(), IPLProcess::version);
            kernel.load_plugin(pluginFilePath.toStdString());

            // we can load all drivers from a specific server
            std::vector<IPLProcessDriver*> drivers = kernel.get_all_drivers<IPLProcessDriver>(IPLProcess::server_name());

            qDebug() << pluginFilePath;
            qDebug() << "server_name: " << QString::fromStdString(IPLProcess::server_name());
            for (std::vector<IPLProcessDriver*>::iterator iter = drivers.begin(); iter != drivers.end(); ++iter) {
                IPLProcessDriver& driver = *(*iter);

                QString className = QString::fromStdString(driver.className());
                QString author = QString::fromStdString(driver.author());
                int version = driver.version();

                qDebug() << "classname: " << className;
                qDebug() << "author: " << author;
                qDebug() << "version: " << version;

                IPLProcess* pluginInstance = driver.create();
                _factory->registerProcess(className, pluginInstance->clone());
                _loadedPlugins.push_back(className);

                IPLImage* testImage = new IPLImage(IPLData::IMAGE_GRAYSCALE, 512, 512);
                pluginInstance->init();
                pluginInstance->processInputData(testImage, 0, false);
                IPLData* result = pluginInstance->getResultData(0);

                qDebug() << "result: " << result->toImage()->height();
            }

            /*
            if (plugin)
            {
                PluginInterface* loadedPlugin = qobject_cast<PluginInterface *>(plugin);
                QString className(typeid(*loadedPlugin->getProcess()).name());
                className = className.split(" ").at(1);

                _factory->registerProcess(className, loadedPlugin->getProcess());

                //qDebug() << pluginPath() + "/" + fileName;
                //_pluginFileSystemWatcher->addPath(pluginPath() + "/" + fileName);

                _loadedPlugins.push_back(loadedPlugin);
                _loaders.push_back(loader);
            }
            else
            {
                QMessageBox::warning(this, "Plugin Error", loader->errorString());
            }*/
        }
    }

    //ui->processTabWidget->init(this);
}

void IPPluginManager::unloadPlugins()
{
    for(int i=0; i<_loadedPlugins.count(); i++)
    {
        // unload instance
        QString plugin = _loadedPlugins.at(i);
        _factory->unregisterProcess(plugin);
    }

    /*for(int i=0; i<_loaders.count(); i++)
    {
        // unload DLL
        QPluginLoader* loader = _loaders.at(i);
        loader->unload();
    }*/

    // delete old tmp directories
    //removeDir(pluginPath() + "/tmp/");

    _loadedPlugins.clear();
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
