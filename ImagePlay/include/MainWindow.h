//#############################################################################
//  File:      MainWindow.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
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

#define IMAGEPLAY_VERSION "20150305 Beta FLOAT x64"

namespace Ui {
class MainWindow;
}

// forward declarations
class ImageViewerWindow;
class IPProcessGridScene;

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
    void                    updateProcessMessages             ();
    void                    loadPlugins                     ();
    void                    unloadPlugins                   ();
    void                    reloadPlugins                   ();
    // disable right click toolbar menu
    QMenu*                  createPopupMenu                 ()                              { return NULL; }
    bool                    synchronizeViews                ()                              { return _synchronizeViews; }
    void                    setDefaultImagePath             (QString path)                  { _defaultImagePath = path; }
    QString                 defaultImagePath                ()                              { return _defaultImagePath; }
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
    void                    clearScene                      ();
    bool                    readProcessFile                 ();
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

private slots:
    void                    on_actionZoomIn_triggered       ();
    void                    on_actionZoomOut_triggered      ();
    void                    on_actionZoomReset_triggered    ();
    void                    on_lineEdit_textChanged         (const QString &arg1);
    void                    on_actionSettings_2_triggered   ();
    void                    on_actionAbout_triggered        ();
    void                    on_buttonBox_rejected           ();
    void                    on_actionPlay_triggered         ();
    void                    on_actionPause_triggered        ();
    void                    on_actionImageViewer_triggered  ();
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

    void on_actionHelp_triggered();

    void on_actionUseOpenCV_toggled(bool value);

    void on_actionShowLog_triggered(bool checked);

    void on_pushButton_clicked();

    void on_actionGeneratePlugin_triggered();

private:
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
    QLabel*                 _progressLabel;
    QTimer*                 _timer;
    QTimer*                 _messageLabelTimer;
    QTimer*                 _autosaveTimer;
    QList<PluginInterface*> _loadedPlugins;
    QList<QPluginLoader*>   _loaders;
    QSettings*              _settings;
    QString                 _currentProcessFileName;
    QString                 _defaultImagePath;
    long                    _lastAutosaveTimestamp;
    int                     _autosaveInterval;
    bool                    _autosaveEnabled;
    bool                    _unsavedChanges;
    bool                    _useOpenCV;
    bool                    _logFileEnabled;
    bool                    _threadRunning;

    // QWidget interface
protected:
    void                    showEvent                       (QShowEvent *);
    void                    closeEvent                      (QCloseEvent*);
    void                    keyReleaseEvent                 (QKeyEvent *);
};

#endif // MAINWINDOW_H




