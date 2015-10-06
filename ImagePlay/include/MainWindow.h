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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>
#include <QMenuBar>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDir>
#include <QFormLayout>
#include <QSpinBox>
#include <QSettings>
#include <QMutableListIterator>
#include <QPluginLoader>

#include "IPProcessStep.h"
#include "IPProcessFactory.h"
#include "IPProcessScript.h"
#include "IPL_processes.h"
#include "ImageViewerWindow.h"
#include "SettingsWindow.h"
#include "AboutWindow.h"
#include "IPProcessGridScene.h"
#include "TutorialDialog.h"
#include "PluginGenerator.h"

#include <map>
#include <string>

#include "IPL_plugininterface.h"

namespace Ui {
class MainWindow;
}

// forward declarations
class ImageViewerWindow;
class IPProcessGridScene;


// constants

#ifndef IMAGEPLAY_VERSION
#define IMAGEPLAY_VERSION "6.0.0-rc.0"
#endif

#ifndef IMAGEPLAY_BUILDDATE
#define IMAGEPLAY_BUILDDATE __DATE__
#endif

#ifndef IMAGEPLAY_BUILDTIME
#define IMAGEPLAY_BUILDTIME __TIME__
#endif

#define MAX_RECENT_PROJECTS 10

//-----------------------------------------------------------------------------
//!MainWindow
/*!
 *
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum MessageType { MESSAGE_SUCCESS, MESSAGE_ERROR, MESSAGE_NEUTRAL };

    explicit                MainWindow                      (QWidget *parent = 0);
                            ~MainWindow                     ();
    void                    logMessage                      (QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void                    writeSettings                   ();
    void                    readSettings                    ();
    //bool                  eventFilter                     (QObject *object, QEvent *event);
    void                    showProcessSettings             (IPProcessStep* processStep);
    void                    hideProcessSettings             ();
    void                    setActiveProcessStep            (IPProcessStep* step);
    void                    showMessage                     (QString msg, MessageType status);
    void                    updateGraphicsView              ();
    void                    updateProcessMessages           ();
    void                    loadPlugins                     ();
    void                    unloadPlugins                   ();
    void                    reloadPlugins                   ();
    bool                    removeDir                       (const QString &dirName);
    void                    setFilterFocus                  ();
    // disable right click toolbar menu
    QMenu*                  createPopupMenu                 ()                              { return NULL; }
    bool                    synchronizeViews                ()                              { return _synchronizeViews; }
    void                    setDefaultImagePath             (QString path)                  { _defaultImagePath = path; }
    QString                 defaultImagePath                ()                              { return _defaultImagePath; }
    void                    setPluginDevPath                (QString path)                  { _pluginDevPath = path; }
    QString                 pluginDevPath                   ()                              { return _pluginDevPath; }
    void                    setPluginPath                   (QString path)                  { _pluginPath = path; }
    QString                 processIconPath                 ()                              { return _processIconPath; }
    QString                 processIconPath                 (QString processID);
    void                    setProcessIconPath              (QString path)                  { _processIconPath = path; }
    QString                 pluginPath                      ();
    void                    setAutosaveEnabled              (bool enabled)                  { _autosaveEnabled = enabled; }
    bool                    autosaveEnabled                 ()                              { return _autosaveEnabled; }
    void                    setLogFileEnabled               (bool enabled)                  { _logFileEnabled = enabled; }
    bool                    logFileEnabled                  ()                              { return _logFileEnabled; }
    void                    setUseOpenCV                    (bool enabled)                  { _useOpenCV = enabled; }
    bool                    useOpenCV                       ()                              { return _useOpenCV; }
    IPProcessFactory*       factory                         ()                              { return _factory; }
    ImageViewerWindow*      imageViewer                     ()                              { return _imageViewer; }
    void                    setAllowChangeActiveProcessStep (bool allow)                    { _allowChangeActiveProcessStep = allow; }
    QList<PluginInterface*>* loadedPlugins                  ()                              { return &_loadedPlugins; }
    void                    addStep                         (IPProcessStep* step);
    void                    removeStep                      (IPProcessStep* step);
    void                    addEdge                         (IPProcessEdge* edge);
    void                    removeEdge                      (IPProcessEdge* edge);
    void                    propagateResultReady            (IPLProcess* process, bool resultReady);
    void                    clearScene                      ();
    bool                    readProcessFile                 ();
    bool                    readProcessFile                 (QString file);
    bool                    writeProcessFile                ();
    void                    lockScene                       ();
    void                    unlockScene                     ();
    void                    loadProcesses                   ();
    void                    setThreadRunning                (bool status)                   { _threadRunning = status; }
    bool                    threadRunning                   ()                              { return _threadRunning; }

public slots:
    void                    execute                         (bool forcedUpdate = false);
    void                    handleProgress                  (int);
    void                    setParamsHaveChanged            ();
    void                    hideMessage                     ();
    void                    on_actionImageViewer_hidden     ();

private slots:
    void                    on_actionZoomIn_triggered       ();
    void                    on_actionZoomOut_triggered      ();
    void                    on_actionZoomReset_triggered    ();
    void                    on_txtFilter_textChanged        (const QString &arg1);
    void                    on_actionSettings_2_triggered   ();
    void                    on_actionAbout_triggered        ();
    void                    on_buttonBox_rejected           ();
    void                    on_actionPlay_triggered         ();
    void                    on_actionPause_triggered        ();
    void                    on_actionSynchronizeViews_triggered(bool checked);
    void                    on_actionTutorial_triggered     ();
    void                    on_actionOpen_triggered         ();
    void                    on_actionSave_triggered         ();
    void                    on_actionSaveAs_triggered       ();
    void                    on_actionShowThumbnails_triggered(bool checked);
    void                    on_sequenceChanged              (int index, int count);
    void                    on_actionSettings_triggered     ();
    void                    on_autosaveTimer                ();
    void                    on_actionNew_triggered          ();
    void                    on_sequenceSlider_valueChanged  (int value);
    void                    on_btnSequenceForward_clicked   ();
    void                    on_btnSequenceBack_clicked      ();
    void                    on_actionHelp_triggered         ();
    void                    on_actionUseOpenCV_toggled      (bool value);
    void                    on_actionShowLog_triggered      (bool checked);
    void                    on_pushButton_clicked           ();
    void                    on_actionGeneratePlugin_triggered();
    void                    on_actionImageViewer_triggered  (bool checked);
    void                    on_btnCloseProcessSettings_clicked();

    void on_actionImage_Viewer_always_on_top_triggered(bool checked);

private:
    void                    addRecentProcessFile(const QString&);
    void                    updateRecentProcessesMenu();

    Ui::MainWindow*         ui;
    IPProcessStep*          _activeProcessStep;
    IPProcessStep*          _lastActiveProcessStep;
    bool                    _allowChangeActiveProcessStep;
    bool                    _synchronizeViews;
    bool                    _isPlaying;
    IPProcessFactory*       _factory;
    ImageViewerWindow*      _imageViewer;
    SettingsWindow*         _settingsWindow;
    IPProcessGridScene*     _scene;
    QTimer*                 _timer;
    QTimer*                 _messageLabelTimer;
    QTimer*                 _autosaveTimer;
    QList<PluginInterface*> _loadedPlugins;
    QList<QPluginLoader*>   _loaders;
    QSettings*              _settings;
    QString                 _currentProcessFileName;
    QString                 _defaultImagePath;
    QString                 _pluginDevPath;
    QString                 _pluginPath;
    QString                 _pluginTmpPath;
    QString                 _processIconPath;
    long                    _lastAutosaveTimestamp;
    int                     _autosaveInterval;
    bool                    _autosaveEnabled;
    bool                    _unsavedChanges;
    bool                    _useOpenCV;
    bool                    _logFileEnabled;
    bool                    _threadRunning;
    QStringList             _recentProcessFiles;

    // QWidget interface
protected:
    void                    showEvent                       (QShowEvent *);
    void                    closeEvent                      (QCloseEvent*);
    void                    keyPressEvent                   (QKeyEvent *);
};

#endif // MAINWINDOW_H




