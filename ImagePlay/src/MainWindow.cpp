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

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _activeProcessStep = NULL;
    _lastActiveProcessStep = NULL;
    _allowChangeActiveProcessStep = false;
    _synchronizeViews = true;
    _isPlaying = false;
    _lastAutosaveTimestamp = 0;
    _autosaveEnabled = true;
    _logFileEnabled = false;
    _autosaveInterval = 10; // 10 seconds
    _unsavedChanges = false;
    _useOpenCV = true;
    _threadRunning = false;

    _imageViewer = new ImageViewerWindow(this);
    _settingsWindow = new SettingsWindow(this);

    _pluginManager = new IPPluginManager();

    _scene = ui->graphicsView->scene();

    // timer
    _timer = new QTimer(this);
    // update and execute graphics view on time out
    connect(_timer, SIGNAL(timeout()), this, SLOT(execute()));

    // autosave timer
    _autosaveTimer = new QTimer(this);
    _autosaveTimer->setInterval(_autosaveInterval*1000);
    connect(_autosaveTimer, &QTimer::timeout, this, &MainWindow::on_autosaveTimer);

    if(_autosaveEnabled)
        _autosaveTimer->start();

    // message timer
    _messageLabelTimer = new QTimer(this);
    ui->messageLabel->hide();

    // sequence control widget
    // (at the moment not working. it was used for the control of imageloading in sequence loader process.)
    //ui->toolBar->addWidget(ui->sequenceControlWidget);
    ui->sequenceControlWidget->setEnabled(false);
    ui->sequenceControlWidget->hide();

    connect(ui->graphicsView, &IPProcessGrid::sequenceChanged, this, &MainWindow::on_sequenceChanged);
    //connect(_pluginFileSystemWatcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::on_pluginDirectoryChanged);
    //connect(_pluginFileSystemTimer, &QTimer::timeout, this, &MainWindow::reloadPlugins);

    // read and apply settings
    readSettings();

    ui->actionSynchronizeViews->setChecked(_synchronizeViews);

    // add version number to title
    setWindowTitle(windowTitle().append(" ").append(IMAGEPLAY_VERSION));

    // connect to MainWindow
    ui->graphicsView->setMainWindow(this);
    ui->processPropertiesWidget->setMainWindow(this);

    tabifyDockWidget(ui->dockProcesses, ui->dockSettings);
    ui->dockProcesses->raise();
    ui->dockSettings->setVisible(false);

    // remove default titlebar
    ui->dockProcesses->setTitleBarWidget(new QWidget);
    ui->dockSettings->setTitleBarWidget(new QWidget);
    ui->dockLog->setTitleBarWidget(new QWidget);

    // load built-in processes
    loadProcesses();

    // load plugin processes
    _pluginManager->loadPlugins(_pluginPath, _factory);

    ui->processTabWidget->init(this);

#ifndef USE_FERVOR_UPDATER
        ui->actionCheck_for_updates->setVisible(false);
#endif

#ifdef Q_OS_MAC
    // set the right placeholder text for OS X
    ui->txtFilter->setPlaceholderText("Find (Cmd+F)...");
#endif

    //set focus on filter lineedit
    setFilterFocus();

    if(QApplication::arguments().length() > 1)
    {
        QString filePath = QApplication::arguments().at(1);
        readProcessFile(filePath);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _factory;
}

void MainWindow::logMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString line;
    int _debugLevel = 1;
    QString timestamp = QTime::currentTime().toString("hh:mm:ss");
    switch (type) {
    case QtDebugMsg:
        line = QString("%1 Debug: %2").arg(timestamp).arg(msg);
        if (ui->dockLog->isVisible() && _debugLevel >= 1)
            ui->txtLog->append(QString("<b>%1</b> %2").arg(timestamp).arg(msg));
        break;
    case QtWarningMsg:
        line = QString("%1 Warning: %2").arg(timestamp).arg(msg);
        if (ui->dockLog->isVisible())
            ui->txtLog->append(QString("<b style=\"color:blue\">%1</b> %2").arg(timestamp).arg(msg));
        break;
    case QtCriticalMsg:
        line = QString("%1 Critical: %2").arg(timestamp).arg(msg);
        if (ui->dockLog->isVisible())
            ui->txtLog->append(QString("<b style=\"color:red\">%1</b> %2").arg(timestamp).arg(msg));
        break;
    case QtFatalMsg:
        line = QString("%1 Fatal: %2").arg(timestamp).arg(msg);
        if (ui->dockLog->isVisible())
            ui->txtLog->append(QString("<b style=\"color:red\">%1 Fatal:</b> %2").arg(timestamp).arg(msg));
        abort();
    }


    // write log file. one file per day
    if(logFileEnabled())
    {
#ifdef Q_OS_WIN
        QDir logDir(qApp->applicationDirPath() + "/log/");
        if(!logDir.exists())
        {
            logDir.mkdir(".");
        }

        QString fileName = logDir.absolutePath();
        fileName += "/";
        fileName += QDate::currentDate().toString("yyyy-MM-dd");
        fileName += ".log";
#else
        QDir logDir("/tmp/");


        QString fileName = logDir.absolutePath();
        fileName += "/ImagePlay.";
        fileName += QDate::currentDate().toString("yyyy-MM-dd");
        fileName += ".log";
#endif

        QFile logFile(fileName);

        if (!logFile.open(QIODevice::Append | QIODevice::Text))
        {
            ui->txtLog->append(logFile.fileName() + QString(" not writable!"));
            return;
        }

        logFile.write(line.append("\n").toLocal8Bit());
    }


    //QApplication::processEvents();
}

void MainWindow::readSettings()
{
    // properties
    _settings = new QSettings("BFH", "ImagePlay");
    _useOpenCV          = _settings->value("OpenCV", true).toBool();
    _autosaveEnabled    = _settings->value("AutoSave", true).toBool();
    _defaultImagePath   = _settings->value("DefaultImagePath", "").toString();
    _logFileEnabled     = _settings->value("LogFile", false).toBool();

#ifdef Q_OS_LINUX
    _processIconPath    = _settings->value("ProcessIconPath",   QString(LINUX_PATH_PREFIX) + "process_icons/").toString();
    _pluginPath         = _settings->value("PluginPath",        QString(LINUX_PATH_PREFIX) + "plugins/").toString();
    _pluginDevPath      = _settings->value("PluginDevPath",     QString(LINUX_PATH_PREFIX) + "plugin_development/").toString();
#else
    _processIconPath    = _settings->value("ProcessIconPath",   QCoreApplication::applicationDirPath() + QString("/process_icons/")).toString();
    _pluginPath         = _settings->value("PluginPath",        QCoreApplication::applicationDirPath() + QString("/plugins/")).toString();
    _pluginDevPath      = _settings->value("PluginDevPath",     QCoreApplication::applicationDirPath() + QString("/plugin_development/")).toString();
#endif

    bool showLog        = _settings->value("showLog", false).toBool();
    ui->dockLog->setVisible(showLog);
    ui->actionShowLog->setChecked(showLog);

    bool alwaysOnTop    = _settings->value("alwaysOnTop", true).toBool();
    ui->actionImage_Viewer_always_on_top->setChecked(alwaysOnTop);
    on_actionImage_Viewer_always_on_top_triggered(alwaysOnTop);

    _synchronizeViews   = _settings->value("synchronizeViews", true).toBool();
    ui->actionSynchronizeViews->setChecked(_synchronizeViews);

    bool showThumbnails = _settings->value("showThumbnails", false).toBool();
    ui->actionShowThumbnails->setChecked(showThumbnails);
    _scene->setShowThumbnails(showThumbnails);

    _settings->beginGroup("MainWindow");
    if(_settings->value("isMaximized", false).toBool())
    {
        showMaximized();
    }
    else
    {
        resize(_settings->value("size", QSize(1080, 700)).toSize());
        move(_settings->value("pos", QPoint(100, 100)).toPoint());
    }
    _settings->endGroup();


    _settings->beginGroup("ImageViewerWindow");
    _imageViewer->resize(_settings->value("size", QSize(800, 600)).toSize());
    _imageViewer->move(_settings->value("pos", QPoint(100, 100)).toPoint());
    _imageViewer->setZoomWidgetMode(_settings->value("zoomWidgetMode", 0).toInt());
    _settings->endGroup();

    ui->actionUseOpenCV->setChecked(_useOpenCV);

    // tutorial
    if(!_settings->value("IgnoreTutorial", 0).toInt())
    {
        on_actionTutorial_triggered();

        // only show once
        _settings->setValue("IgnoreTutorial", 1);
    }

    // recent projects
    if (_settings->contains("recentProjects"))
    {
        QVariant storedRecentProjects = _settings->value("recentProjects");
        if (storedRecentProjects.convert(QMetaType::QStringList))
        {
            _recentProcessFiles = storedRecentProjects.value<QStringList>();

            updateRecentProcessesMenu();
        }
        else
        {
            _settings->remove("recentProjects");
        }
    }
}

void MainWindow::writeSettings()
{
    _settings->setValue("OpenCV",           _useOpenCV);
    _settings->setValue("AutoSave",         _autosaveEnabled);
    _settings->setValue("DefaultImagePath", _defaultImagePath);
    _settings->setValue("PluginDevPath",    _pluginDevPath);
    _settings->setValue("PluginPath",       _pluginPath);
    _settings->setValue("LogFile",          _logFileEnabled);

    bool showLog = ui->actionShowLog->isChecked();
    bool synchronizeViews = ui->actionSynchronizeViews->isChecked();
    bool showThumbnails = ui->actionShowThumbnails->isChecked();
    bool alwaysOnTop = ui->actionImage_Viewer_always_on_top->isChecked();
    _settings->setValue("showLog",          showLog);
    _settings->setValue("alwaysOnTop",      alwaysOnTop);
    _settings->setValue("synchronizeViews", synchronizeViews);
    _settings->setValue("showThumbnails",   showThumbnails);

    _settings->beginGroup("MainWindow");
    _settings->setValue("size",             size());
    _settings->setValue("pos",              pos());
    _settings->setValue("isMaximized",      isMaximized());
    _settings->endGroup();

    QSize   size = _imageViewer->lastSize();
    QPoint  pos  = _imageViewer->lastPos();
    int     zoomWidgetMode = _imageViewer->zoomWidgetMode();
    if(size.width() > 0 && size.height() > 0)
    {
        _settings->beginGroup("ImageViewerWindow");
        _settings->setValue("size", size);
        _settings->setValue("pos",  pos);
        _settings->setValue("zoomWidgetMode",  zoomWidgetMode);
        _settings->endGroup();
    }

    // recent projects
    if (_recentProcessFiles.count() > 0 || _settings->contains("recentProjects"))
    {
        _settings->setValue("recentProjects", QVariant::fromValue<QStringList>(_recentProcessFiles));
    }
}

void MainWindow::showProcessSettings(IPProcessStep* processStep)
{
    ui->dockSettings->setVisible(true);
    ui->dockProcesses->setVisible(false);

    // add description
    if(processStep->process()->description().length() > 0)
    {
        ui->lblProcessDescription->setText(QString::fromStdString(processStep->process()->description()));
        ui->lblProcessDescription->setVisible(true);
    }
    else
    {
        ui->lblProcessDescription->setVisible(false);
    }

    // change title
    QString title = QString::fromStdString(processStep->process()->title());
    ui->lblProcessSettings->setText(title);

    // hide help button because it is currently not used
    //ui->btnHelpPage->hide();

    // add inputs/outputs
    if(processStep->process()->inputs()->size() > 0 || processStep->process()->outputs()->size() > 0)
    {
        QString msgInputsOutputs;

        if(processStep->process()->inputs()->size() > 0)
        {
            msgInputsOutputs.append("<b>Inputs:</b>");
            for(int i=0; i < (int)processStep->process()->inputs()->size(); i++)
            {
                IPLProcessIO input = processStep->process()->inputs()->at(i);
                QString msgString("<br />%1: %2 (<i>%3</i>)");
                msgInputsOutputs.append(msgString
                                        .arg(input.index)
                                        .arg(QString::fromStdString(input.name))
                                        .arg(QString::fromStdString(dataTypeName(input.type))));
            }
        }

        if(processStep->process()->outputs()->size() > 0)
        {
            if(processStep->process()->inputs()->size() > 0)
                msgInputsOutputs.append("<br />");

            msgInputsOutputs.append("<b>Outputs:</b>");
            for(int i=0; i < (int)processStep->process()->outputs()->size(); i++)
            {
                IPLProcessIO output = processStep->process()->outputs()->at(i);
                QString msgString("<br />%1: %2 (<i>%3</i>)");
                msgInputsOutputs.append(msgString
                                        .arg(output.index)
                                        .arg(QString::fromStdString(output.name))
                                        .arg(QString::fromStdString(dataTypeName(output.type))));
            }
        }
        ui->lblProcessInputsOutputs->setText(msgInputsOutputs);
        ui->lblProcessInputsOutputs->setVisible(true);
    }
    else
    {
        ui->lblProcessInputsOutputs->setVisible(false);
    }

    ui->processPropertiesWidget->init(processStep);
    ui->processMessageWidget->init(processStep);
}

void MainWindow::hideProcessSettings()
{
    ui->processPropertiesWidget->closeSettings();

    ui->dockSettings->setVisible(false);
    ui->dockProcesses->setVisible(true);
}

void MainWindow::setActiveProcessStep(IPProcessStep* step)
{
    if(!step)
    {
        _activeProcessStep = NULL;
        _lastActiveProcessStep = NULL;
        return;
    }

    if(_allowChangeActiveProcessStep)
    {
        _activeProcessStep = step;

        if(_synchronizeViews)
        {
            _allowChangeActiveProcessStep = false;
            _imageViewer->setActiveStep(_activeProcessStep->stepID());
            _allowChangeActiveProcessStep = true;
        }

        if(_lastActiveProcessStep)
        {
            hideProcessSettings();
            _lastActiveProcessStep->setEditing(false);
        }

        // activate and show settings
        showProcessSettings(_activeProcessStep);
        _activeProcessStep->setEditing(true);

        // save last active step
        _lastActiveProcessStep = _activeProcessStep;
    }
}

void MainWindow::updateGraphicsView()
{
    // force update
    ui->graphicsView->viewport()->update();
}

void MainWindow::updateProcessMessages()
{
    ui->processMessageWidget->updateMessages();
}

void MainWindow::loadProcesses()
{
    // we need a process factory to instanciate at runtime from string
    _factory = new IPProcessFactory;

    // register all processes to the factory
    _factory->registerProcess("IPLConvertToGray",       new IPLConvertToGray);
    _factory->registerProcess("IPLConvertToColor",      new IPLConvertToColor);
    _factory->registerProcess("IPLBinarize",            new IPLBinarize);
    _factory->registerProcess("IPLLoadImage",           new IPLLoadImage);
    _factory->registerProcess("IPLCamera",              new IPLCamera);
    //_factory->registerProcess("IPLLoadVideo",           new IPLLoadVideo);
    _factory->registerProcess("IPLLoadImageSequence",   new IPLLoadImageSequence);
    _factory->registerProcess("IPLScreenshot",          new IPLScreenshot);
    _factory->registerProcess("IPLSaveImage",           new IPLSaveImage);
    _factory->registerProcess("IPLSplitPlanes",         new IPLSplitPlanes);
    _factory->registerProcess("IPLMergePlanes",         new IPLMergePlanes);
    _factory->registerProcess("IPLGaussianLowPass",     new IPLGaussianLowPass);
    _factory->registerProcess("IPLGammaCorrection",     new IPLGammaCorrection);
    _factory->registerProcess("IPLConvolutionFilter",   new IPLConvolutionFilter);
    _factory->registerProcess("IPLMorphologyBinary",    new IPLMorphologyBinary);
    _factory->registerProcess("IPLMorphologyGrayscale", new IPLMorphologyGrayscale);
    _factory->registerProcess("IPLMorphologyHitMiss",   new IPLMorphologyHitMiss);
    _factory->registerProcess("IPLBlendImages",         new IPLBlendImages);
    _factory->registerProcess("IPLArithmeticOperations",new IPLArithmeticOperations);
    _factory->registerProcess("IPLArithmeticOperationsConstant", new IPLArithmeticOperationsConstant);
    _factory->registerProcess("IPLSynthesize",          new IPLSynthesize);
    _factory->registerProcess("IPLFlipImage",           new IPLFlipImage);
    _factory->registerProcess("IPLGradientOperator",    new IPLGradientOperator);
//    TODO: Fix algorithm and add again.
    _factory->registerProcess("IPLRandomPoint",         new IPLRandomPoint);
    _factory->registerProcess("IPLCanvasSize",          new IPLCanvasSize);
    _factory->registerProcess("IPLResize",              new IPLResize);
    _factory->registerProcess("IPLRotate",              new IPLRotate);

    _factory->registerProcess("IPLEnhanceMode",         new IPLEnhanceMode);
    _factory->registerProcess("IPLFillConcavities",     new IPLFillConcavities);
    _factory->registerProcess("IPLGabor",               new IPLGabor);
    _factory->registerProcess("IPLInverseContrastRatioMapping",new IPLInverseContrastRatioMapping);
    _factory->registerProcess("IPLMax",                 new IPLMax);
    _factory->registerProcess("IPLMaxMinMedian",        new IPLMaxMinMedian);
    _factory->registerProcess("IPLMedian",              new IPLMedian);
    _factory->registerProcess("IPLCanny",               new IPLCanny);
    _factory->registerProcess("IPLHoughCircles",        new IPLHoughCircles);
    _factory->registerProcess("IPLHarrisCorner",        new IPLHarrisCorner);
    _factory->registerProcess("IPLExtractLines",        new IPLExtractLines);
    _factory->registerProcess("IPLExtrema",             new IPLExtrema);
    _factory->registerProcess("IPLLaplaceOfGaussian",   new IPLLaplaceOfGaussian);
    _factory->registerProcess("IPLMin",                 new IPLMin);
    _factory->registerProcess("IPLMorphologicalEdge",   new IPLMorphologicalEdge);
    _factory->registerProcess("IPLNormalizeIllumination",new IPLNormalizeIllumination);
    _factory->registerProcess("IPLBinarizeSavola",      new IPLBinarizeSavola);
    _factory->registerProcess("IPLOnePixelEdge",        new IPLOnePixelEdge);
    _factory->registerProcess("IPLRankTransform",       new IPLRankTransform);
    _factory->registerProcess("IPLUnsharpMasking",      new IPLUnsharpMasking);
    _factory->registerProcess("IPLCompassMask",         new IPLCompassMask);

    _factory->registerProcess("IPLTriangleSegmentation",new IPLTriangleSegmentation);
    _factory->registerProcess("IPLStretchContrast",     new IPLStretchContrast);
    _factory->registerProcess("IPLNegate",              new IPLNegate);
    _factory->registerProcess("IPLMarkImage",           new IPLMarkImage);
    _factory->registerProcess("IPLLocalThreshold",      new IPLLocalThreshold);
    _factory->registerProcess("IPLHysteresisThreshold", new IPLHysteresisThreshold);
    _factory->registerProcess("IPLFalseColor",          new IPLFalseColor);
    _factory->registerProcess("IPLEqualizeHistogram",   new IPLEqualizeHistogram);
    _factory->registerProcess("IPLBinarizeUnimodal",    new IPLBinarizeUnimodal);
    _factory->registerProcess("IPLBinarizeOtsu",        new IPLBinarizeOtsu);
    _factory->registerProcess("IPLBinarizeKMeans",      new IPLBinarizeKMeans);
    _factory->registerProcess("IPLBinarizeEntropy",     new IPLBinarizeEntropy);
    _factory->registerProcess("IPLAddNoise",            new IPLAddNoise);

    _factory->registerProcess("IPLFFT",                 new IPLFFT);
    _factory->registerProcess("IPLIFFT",                new IPLIFFT);
    _factory->registerProcess("IPLFrequencyFilter",     new IPLFrequencyFilter);

    _factory->registerProcess("IPLLabelBlobs",          new IPLLabelBlobs);

    _factory->registerProcess("IPLAccumulate",          new IPLAccumulate);
    _factory->registerProcess("IPLHoughLines",          new IPLHoughLines);
    _factory->registerProcess("IPLHoughLineSegments",   new IPLHoughLineSegments);

    _factory->registerProcess("IPLUndistort",           new IPLUndistort);
    _factory->registerProcess("IPLWarpAffine",          new IPLWarpAffine);
    _factory->registerProcess("IPLWarpPerspective",     new IPLWarpPerspective);

    _factory->registerProcess("IPLGoodFeaturesToTrack", new IPLGoodFeaturesToTrack);

    // not ready:
    /*_factory->registerProcess("IPLMatchTemplate",       new IPLMatchTemplate);
    _factory->registerProcess("IPLFloodFill",           new IPLFloodFill);

    _factory->registerProcess("IPLOpticalFlow",         new IPLOpticalFlow);

    _factory->registerProcess("IPProcessScript",        new IPProcessScript);

    _factory->registerProcess("IPLFeatureDetection",    new IPLFeatureDetection);
    _factory->registerProcess("IPLFeatureMatcher",      new IPLFeatureMatcher);

    _factory->registerProcess("IPLCameraCalibration",   new IPLCameraCalibration);*/
}

void MainWindow::reloadPlugins()
{
    // don't try to reload while running
    if(ui->graphicsView->isRunning())
        return;

    setActiveProcessStep(NULL);
    _allowChangeActiveProcessStep = false;
    writeProcessFile();
    clearScene();
    _pluginManager->unloadPlugins();
    _pluginManager->loadPlugins(_pluginPath, _factory);
    readProcessFile();

    // activate plugin tab
    ui->processTabWidget->setCurrentIndex(ui->processTabWidget->count()-1);

    // refresh process library
    ui->processTabWidget->init(this);

    // switch to last tab again
    ui->processTabWidget->setCurrentIndex(ui->processTabWidget->count()-1);

    _allowChangeActiveProcessStep = true;
}

void MainWindow::unloadPlugins()
{
    // don't try to reload while running
    if(ui->graphicsView->isRunning())
        return;

    setActiveProcessStep(NULL);
    _allowChangeActiveProcessStep = false;
    writeProcessFile();
    clearScene();
    _pluginManager->unloadPlugins();
    readProcessFile();

    // activate plugin tab
    ui->processTabWidget->setCurrentIndex(ui->processTabWidget->count()-1);

    // refresh process library
    ui->processTabWidget->init(this);

    // switch to last tab again
    ui->processTabWidget->setCurrentIndex(ui->processTabWidget->count()-1);

    _allowChangeActiveProcessStep = true;
}

void MainWindow::setFilterFocus()
{
    ui->txtFilter->setFocus();
    ui->txtFilter->setSelection(0, ui->txtFilter->text().length());
}

QString MainWindow::processIconPath(QString processID)
{
    return processIconPath() + processID + QString(".png");
}

QString MainWindow::pluginPath()
{
    if(_pluginPath.length() < 1)
        return QCoreApplication::applicationDirPath() + "/plugins/";
    else
        return _pluginPath;
}

void MainWindow::showMessage(QString msg, MessageType status)
{
    if(status == MainWindow::MESSAGE_SUCCESS)
    {
        ui->statusBar->setStyleSheet("background: #3ad179; font-weight: bold;");
    }
    else if(status == MainWindow::MESSAGE_ERROR)
    {
        ui->statusBar->setStyleSheet("background: #e74c3c; font-weight: bold;");
    }
    else
    {
        ui->statusBar->setStyleSheet("background: none; font-weight: bold;");
    }
    ui->statusBar->showMessage(msg);

    _messageLabelTimer->start(3000);
    connect(_messageLabelTimer, &QTimer::timeout, this, &MainWindow::hideMessage);
}

void MainWindow::addStep(IPProcessStep *step)
{
    _allowChangeActiveProcessStep = false;

    // add to scene
    _scene->addStep(step);

    // add to image viewer
    _imageViewer->addProcessStep(step);

    _allowChangeActiveProcessStep = true;

    // update graphics
    setParamsHaveChanged();
//    execute();

}

void MainWindow::removeStep(IPProcessStep *step)
{
    // hide settings
    hideProcessSettings();

    if(_activeProcessStep == step)
        _activeProcessStep= NULL;

    if(_lastActiveProcessStep == step)
        _lastActiveProcessStep = NULL;

    // remove from image viewer
    _imageViewer->removeProcessStep(step);

    // remove from scene
    _scene->removeStep(step);

    // delete step;

    setParamsHaveChanged();
//    execute();
}

void MainWindow::addEdge(IPProcessEdge *edge)
{
    // add to scene
    if(_scene->addEdge(edge))
    {
        // show message
        showMessage("Connected", MainWindow::MESSAGE_SUCCESS);

        // update graphics
        edge->to()->process()->requestUpdate();
        ui->graphicsView->propagateNeedsUpdate(edge->to()->process());
        setParamsHaveChanged();
        execute();
    }
    else
    {
        // show message
        showMessage("Connection not possible: Output and input not compatible.", MainWindow::MESSAGE_ERROR);
    }
}

void MainWindow::removeEdge(IPProcessEdge *edge)
{
    if(edge->to()->process())
        propagateResultReady(edge->to()->process(), false);

    // remove from scene
    _scene->removeEdge(edge);

    // delete edge;

    // update graphics
    setParamsHaveChanged();
    execute();
}

void MainWindow::propagateResultReady(IPLProcess *process, bool resultReady)
{
    ui->graphicsView->propagateResultReady(process, resultReady);
}

void MainWindow::clearScene()
{
    // WARNING: the it doesn't need to increment actually, this is no bug!
    for(auto it = _scene->edges()->begin(); it != _scene->edges()->end(); it)
    {
        removeEdge(*it);
    }
    for(auto it = _scene->steps()->begin(); it != _scene->steps()->end(); it)
    {
        removeStep(*it);
    }
}

bool MainWindow::readProcessFile()
{
    _isPlaying = false;
    _timer->stop();

    QString errorString;

    QString fileName = _currentProcessFileName;

    if(fileName.length() < 1)
    {
        fileName = _pluginPath + "/_autosave.ipj";
    }

    // read json file
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QString fileContents;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        fileContents.append(line);
    }
    file.close();

    // clear current process
    _allowChangeActiveProcessStep = false;

    clearScene();

    // parse file and create process
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(fileContents.toLatin1(), &error);
    qWarning() << error.errorString();

    QJsonObject root = document.object();

    QMap<long, long> stepIdMap;

    QJsonArray steps = root.value("steps").toArray();
    for(auto it = steps.begin(); it != steps.end(); ++it)
    {
        QJsonObject stepObject = (*it).toObject();

        long ID = stepObject.value("ID").toDouble();
        int posX = stepObject.value("posX").toDouble();
        int posY = stepObject.value("posY").toDouble();
        QString type = stepObject.value("type").toString();

        IPProcessStep* newStep = new IPProcessStep(this, type);
        newStep->setPos(QPointF(posX, posY));

        // check if valid
        if(!newStep->process())
        {
            errorString.append("Invalid Process Type: ").append(type).append("\n");
            continue;
        }

        // set the properties
        QJsonArray properties = stepObject.value("properties").toArray();
        for(auto it2 = properties.begin(); it2 != properties.end(); ++it2)
        {
            QJsonObject propertyObject = (*it2).toObject();
            QString  key     = propertyObject.value("key").toString();
            QString  type    = propertyObject.value("type").toString();
            QString  widget    = propertyObject.value("widget").toString();
            QVariant value   = propertyObject.value("value").toVariant();

            IPLProcessProperty* processProperty = (IPLProcessProperty*) newStep->process()->property(key.toStdString());
            if(!processProperty)
            {
                qWarning() << "Invalid process property: " << key;
                continue;
            }

            IPLProcessProperty::SerializedData data;
            data.type = type.toStdString();
            data.widget = widget.toStdString();
            data.value = value.toString().toStdString();

            try
            {
                processProperty->deserialize(data);
            }
            catch(IPLProcessProperty::DeserialationFailed)
            {
                qWarning() << "Deserialation failed: " << key;
                continue;
            }
        }

        // we need to map the file step ID to the new ID
        stepIdMap.insert(ID, newStep->stepID());

        addStep(newStep);
    }

    QJsonArray edges = root.value("edges").toArray();
    for(auto it = edges.begin(); it != edges.end(); ++it)
    {
        QJsonObject edgeObject = (*it).toObject();

        long from       = edgeObject.value("from").toDouble();
        long to         = edgeObject.value("to").toDouble();
        int indexFrom   = edgeObject.value("indexFrom").toDouble();
        int indexTo     = edgeObject.value("indexTo").toDouble();

        long fromID = stepIdMap[from];
        long toID = stepIdMap[to];

        // find step objects
        IPProcessStep* stepFrom = NULL;
        IPProcessStep* stepTo = NULL;

        for(auto it2 = _scene->steps()->begin(); it2 != _scene->steps()->end(); ++it2)
        {
            IPProcessStep* findStep = *it2;
            if(findStep->stepID() == fromID)
                stepFrom = findStep;
            if(findStep->stepID() == toID)
                stepTo = findStep;
        }

        if(stepFrom && stepTo)
        {
            IPProcessEdge* newEdge = new IPProcessEdge(stepFrom, stepTo, indexFrom, indexTo);
            addEdge(newEdge);
        }
    }

    // display error message
    if(errorString.length() > 0)
    {
        QMessageBox::warning(this, "File Errors", errorString);
    }

    ui->graphicsView->execute(true);

    _allowChangeActiveProcessStep = true;

    // restart if the user has not paused
    if(ui->actionPause->isVisible())
    {
        _isPlaying = true;
        _timer->start(66); // 33ms = 30 fps
    }
    return true;
}

void MainWindow::setCurrentFile(QString file)
{
    _currentProcessFileName = file;
    IPLFileIO::setBasedir(QFileInfo(_currentProcessFileName).absolutePath().toStdString());

    //setWindowFilePath(_currentProcessFileName);
    QString windowTitle("ImagePlay");
    if(_currentProcessFileName.length() > 0)
        windowTitle.append(" - ").append(_currentProcessFileName);
    setWindowTitle(windowTitle);
}

bool MainWindow::readProcessFile(QString file)
{
    setCurrentFile(file);
    return readProcessFile();
}

bool MainWindow::writeProcessFile()
{
    QString fileName = _currentProcessFileName;

    if(fileName.length() < 1)
    {
        fileName = _pluginPath + "/_autosave.ipj";
    }

    QJsonDocument document;
    QJsonObject root;

    QDateTime now = QDateTime::currentDateTime();
    root.insert("timestamp", QJsonValue::fromVariant(now.toTime_t()));

    QString username = QString::fromLatin1(getenv("USERNAME"));
    root.insert("author", QJsonValue::fromVariant(username));

    // collect steps
    QJsonArray steps;
    for(int i=0; i<_scene->steps()->length(); i++)
    {
        IPProcessStep* s = _scene->steps()->at(i);
        QJsonObject step;
        step.insert("ID",   QJsonValue::fromVariant(s->stepID()));
        step.insert("type", QJsonValue::fromVariant(QString::fromStdString(s->process()->className())));
        step.insert("posX", QJsonValue::fromVariant(s->pos().x()));
        step.insert("posY", QJsonValue::fromVariant(s->pos().y()));

        QJsonArray properties;
        IPLProcessPropertyMap* propertyMap = s->process()->properties();
        for(auto it = propertyMap->begin(); it != propertyMap->end(); ++it)
        {
            QString key = QString::fromStdString(it->first);

            auto &p = it->second;
            QString propertyJsonString;
            {
                IPLProcessProperty::SerializedData data = p->serialize();
                std::ostringstream json;
                json << "{\n";
                json << " \"type\": \"" << data.type << "\",\n";
                json << " \"widget\": \"" << data.widget << "\",\n";
                json << " \"widgetName\": \"" << data.widgetName << "\",\n";
                json << " \"value\": \"" << data.value << "\"\n";
                json << "}";
               propertyJsonString = QString::fromStdString(json.str());
            }

            QJsonParseError error;
            QJsonDocument tmp = QJsonDocument::fromJson(propertyJsonString.toLatin1(), &error);
            if(tmp.isObject())
            {
                QJsonObject property = tmp.object();
                property.insert("key", key);
                properties.append(property);
            }
        }

        step.insert("properties", properties);

        steps.append(step);
    }
    root.insert("steps", steps);


    // collect edges
    QJsonArray edges;
    for(int i=0; i<_scene->edges()->length(); i++)
    {
        IPProcessEdge* e = _scene->edges()->at(i);
        QJsonObject edge;
        edge.insert("from", QJsonValue::fromVariant(e->from()->stepID()));
        edge.insert("to", QJsonValue::fromVariant(e->to()->stepID()));
        edge.insert("indexFrom", QJsonValue::fromVariant(e->indexFrom()));
        edge.insert("indexTo", QJsonValue::fromVariant(e->indexTo()));

        edges.append(edge);
    }
    root.insert("edges", edges);

    document.setObject(root);

    // write json to the file
    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString msg("Error while saving process file: ");
        msg.append(file.fileName());
        showMessage(msg, MESSAGE_ERROR);
        return false;
    }

    QTextStream out(&file);

    out << document.toJson();

    file.close();

    QString msg("Process file successfully saved: ");
    msg.append(file.fileName());
    showMessage(msg, MESSAGE_SUCCESS);

    _unsavedChanges = false;

    return true;
}

void MainWindow::lockScene()
{
    //_imageViewer->setEnabled(false);
    _imageViewer->setIgnoreMouseEvents(true);
    ui->graphicsView->setEnabled(false);
}

void MainWindow::unlockScene()
{
    //_imageViewer->setEnabled(true);
    _imageViewer->setIgnoreMouseEvents(false);
    ui->graphicsView->setEnabled(true);
}

void MainWindow::hideMessage()
{
    _messageLabelTimer->stop();
    ui->statusBar->clearMessage();
    ui->statusBar->setStyleSheet("background: none; font-weight: bold;");
}

void MainWindow::execute(bool forcedUpdate/* = false */)
{
    if(_isPlaying)
    {
        if(forcedUpdate)
            ui->graphicsView->requestUpdate();

        ui->graphicsView->execute(forcedUpdate);
    }
}

void MainWindow::setParamsHaveChanged()
{
    _unsavedChanges = true;
    ui->graphicsView->requestUpdate();
}

void MainWindow::handleProgress(int progress)
{
    qDebug() << "Progress: " << progress;
}

/*
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *mouseEvent = static_cast< QMouseEvent* >(event);
        int index = ui->tabWidget->tabBar()->tabAt(mouseEvent->pos());
        QLabel* titleLabel = (QLabel*) ui->dockProcesses->titleBarWidget();
//        QString tabText = ui->tabWidget->
//        titleLabel->setText(QString::number(index).append(": ").append(tabText));
    }
    else
    {
        QLabel* titleLabel = (QLabel*) ui->dockProcesses->titleBarWidget();
        titleLabel->setText(QString(""));
    }
    return false;
}*/


void MainWindow::on_actionZoomIn_triggered()
{
    ui->graphicsView->zoomIn();
}

void MainWindow::on_actionZoomOut_triggered()
{
    ui->graphicsView->zoomOut();
}


void MainWindow::on_actionZoomReset_triggered()
{
    ui->graphicsView->zoomTo(1.0);
}


void MainWindow::on_txtFilter_textChanged(const QString &text)
{
    ui->processTabWidget->filter(text);
}

void MainWindow::on_buttonBox_rejected()
{
    ui->processPropertiesWidget->closeSettings();

    ui->dockSettings->setVisible(false);
    ui->dockProcesses->setVisible(true);
}

void MainWindow::on_actionSettings_2_triggered()
{
    SettingsWindow* settingsWindow = new SettingsWindow(this);
    settingsWindow->show();
}

void MainWindow::on_actionAbout_triggered()
{
    AboutWindow* aboutWindow = new AboutWindow(this);
    aboutWindow->show();
}

void MainWindow::on_actionPlay_triggered()
{
    _isPlaying = true;

    // show and hide buttons
    ui->actionPause->setVisible(true);
    ui->actionPlay->setVisible(false);

    // force one execution
    setParamsHaveChanged();

    //execute();
    _timer->start(66); // 33ms = 30 fps
    _imageViewer->show();
    ui->actionImageViewer->setChecked(true);

    // apply settings
    /*_settings->beginGroup("ImageViewerWindow");
    _imageViewer->resize(_settings->value("size", QSize(1080, 700)).toSize());
    _imageViewer->move(_settings->value("pos", QPoint(100, 100)).toPoint());
    _settings->endGroup();*/
}

void MainWindow::on_actionPause_triggered()
{
    _isPlaying = false;

    // show and hide buttons
    ui->actionPause->setVisible(false);
    ui->actionPlay->setVisible(true);

    _timer->stop();
}


void MainWindow::on_actionSynchronizeViews_triggered(bool checked)
{
    _synchronizeViews = checked;
}


void MainWindow::on_actionTutorial_triggered()
{
    TutorialDialog* tutorial = new TutorialDialog(this);
    tutorial->show();
}

void MainWindow::on_actionSave_triggered()
{
    // if no filename, then ask
    if(_currentProcessFileName.length() == 0)
    {
            setCurrentFile(QFileDialog::getSaveFileName(this, tr("Save File"), "", "ImagePlay JSON File (*.ipj)"));
    }
    writeProcessFile();

    addRecentProcessFile(_currentProcessFileName);
}

void MainWindow::on_actionSaveAs_triggered()
{
    _currentProcessFileName.clear();
    on_actionSave_triggered();

    addRecentProcessFile(_currentProcessFileName);
}

void MainWindow::on_actionOpen_triggered()
{
    setCurrentFile(QFileDialog::getOpenFileName(this, tr("Open File"), "", "ImagePlay JSON File (*.ipj)"));

    readProcessFile();

    addRecentProcessFile(_currentProcessFileName);
}

void MainWindow::addRecentProcessFile(const QString& recentFile)
{
    QString recentFilePath = recentFile.trimmed();

    if (recentFilePath.length() && !_recentProcessFiles.contains(recentFilePath))
    {
        _recentProcessFiles.insert(0, recentFilePath);

        if (_recentProcessFiles.count() > MAX_RECENT_PROJECTS)
        {
            _recentProcessFiles.erase(
                _recentProcessFiles.begin() + MAX_RECENT_PROJECTS, _recentProcessFiles.end());
        }

        writeSettings();

        updateRecentProcessesMenu();
    }
}

void MainWindow::updateRecentProcessesMenu()
{
    ui->menuOpenRecent->clear();

    for (int index = 0; index < _recentProcessFiles.count(); ++index)
    {
        QString processFile = _recentProcessFiles[index];
        QAction* action = new QAction(processFile, ui->menuOpenRecent);
        QObject::connect(action, &QAction::triggered, [this, index](bool checked){
            if (index < _recentProcessFiles.count()) {
                QString processFile = _recentProcessFiles[index];
                QFileInfo processFileInfo(processFile);
                if (processFileInfo.exists()) {
                    setCurrentFile(processFile);
                    readProcessFile();
                }
            }
        });
        ui->menuOpenRecent->addAction(action);
    }

    if (_recentProcessFiles.count() > 0)
    {
        ui->menuOpenRecent->addSeparator();
        QAction* clearAction = new QAction(tr("Clear"), ui->menuOpenRecent);
        QObject::connect(clearAction, &QAction::triggered, [=](bool checked){
            _recentProcessFiles.clear();
            writeSettings();
            updateRecentProcessesMenu();
        });
        ui->menuOpenRecent->addAction(clearAction);
    }
}

void MainWindow::showEvent(QShowEvent*)
{
    // autostart
    on_actionPlay_triggered();
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    QMessageBox question(QMessageBox::Question, "Quit", "Do you want to quit ImagePlay?", QMessageBox::Yes | QMessageBox::No, this);
    question.setIconPixmap(QPixmap(":/question.png").scaledToWidth(48, Qt::SmoothTransformation));

    int reply = question.exec();
    if (reply == QMessageBox::Yes)
    {
        writeSettings();

        ui->graphicsView->stopExecution();
        ui->graphicsView->terminate();
        //_pluginManager->unloadPlugins();

        //TODO: Application crashed here (update calls after the deallocation). Further investigation might be necessary.
//        delete _imageViewer;
//        _imageViewer->deleteLater();
//        _imageViewer = NULL;

        // try closing any videocapture items
        IPLCameraIO::release();

        QApplication::quit();
    }
    else
    {
        e->ignore();
    }
}


void MainWindow::on_actionShowThumbnails_triggered(bool checked)
{
    _scene->setShowThumbnails(checked);
    updateGraphicsView();
}

void MainWindow::on_sequenceChanged(int index, int count)
{
    ui->sequenceControlWidget->setEnabled(true);
    ui->sequenceSlider->setMaximum(count-1);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    Qt::KeyboardModifiers modifiers = QGuiApplication::keyboardModifiers();

    if(event->key() == Qt::Key_Escape)
    {
        hideProcessSettings();
    }
    if(event->key() == Qt::Key_F && modifiers&Qt::ControlModifier)
    {
        setFilterFocus();
    }
}

void MainWindow::on_actionSettings_triggered()
{
    _settingsWindow->show();
}

void MainWindow::on_autosaveTimer()
{
    if(_autosaveEnabled)
        writeProcessFile();

    writeSettings();
}

void MainWindow::on_actionNew_triggered()
{
    if(_unsavedChanges)
    {
        QMessageBox question(QMessageBox::Question, "Clear Scene", "You have unsaved changes, do you want to clear the process scene?", QMessageBox::Yes | QMessageBox::No, this);
        question.setIconPixmap(QPixmap(":/question.png").scaledToWidth(48, Qt::SmoothTransformation));
        int reply = question.exec();
        if (reply == QMessageBox::No)
            return;
    }

    clearScene();
    setCurrentFile("");
}


void MainWindow::on_sequenceSlider_valueChanged(int value)
{
    ui->graphicsView->setSequenceIndex(value);

    QString msg("%1/%2");
    msg = msg.arg(ui->sequenceSlider->value()).arg(ui->sequenceSlider->maximum());
    ui->sequenceLabel->setText(msg);
}

void MainWindow::on_btnSequenceForward_clicked()
{
    // next
    ui->sequenceSlider->setValue(ui->sequenceSlider->value()+1);
}


void MainWindow::on_btnSequenceBack_clicked()
{
    // previous
    ui->sequenceSlider->setValue(ui->sequenceSlider->value()-1);
}

/*!
 * Opens the HTML-Help file in the default browser
 */
void MainWindow::on_actionHelp_triggered()
{
    //QUrl url = QUrl::fromLocalFile("doc/html/index.html");
    /*QUrl url = QUrl::fromLocalFile("D:/Projects/imageplay_prototypes/_bin/Release/x64/doc/CPVR-ImageProcessing-Script.pdf");
    url.setFragment("3.1.3 Abbildungsfehler (Aberration)");
    qDebug() << url.toLocalFile();
    qDebug() << url.toString();*/
    //QDesktopServices::openUrl(url);
}

void MainWindow::on_actionUseOpenCV_toggled(bool value)
{
    _useOpenCV = value;
    ui->graphicsView->scene()->update();

    // force one execution
    execute(true);
}

void MainWindow::on_actionShowLog_triggered(bool checked)
{
    ui->dockLog->setVisible(checked);
}

void MainWindow::on_pushButton_clicked()
{
    QUrl url = QUrl::fromLocalFile("doc/html/index.html");
    //QUrl url = QUrl::fromLocalFile("doc/CPVR-ImageProcessing-Script.pdf");
    //url.setFragment("page=3");
    qDebug() << url.toString();
    QDesktopServices::openUrl(url);
}

void MainWindow::on_actionGeneratePlugin_triggered()
{
    if(_pluginDevPath.length() == 0)
    {
        QMessageBox::warning(this, "Plugin Error", "Plugin Dev Path is not set.");
        _settingsWindow->show();
        return;
    }
    if(_pluginPath.length() == 0)
    {
        QMessageBox::warning(this, "Plugin Error", "Plugin Path is not set.");
        _settingsWindow->show();
        return;
    }

    PluginGenerator* pluginGenerator = new PluginGenerator(this, _pluginDevPath, _pluginPath, QCoreApplication::applicationDirPath());
    pluginGenerator->show();
}

void MainWindow::on_actionImageViewer_triggered(bool checked)
{
    _imageViewer->setVisible(checked);
}

void MainWindow::on_actionImageViewer_hidden()
{
    ui->actionImageViewer->setChecked(false);
}

void MainWindow::on_btnCloseProcessSettings_clicked()
{
    hideProcessSettings();
}

void MainWindow::on_btnResetProcessSettings_clicked()
{
    ui->processPropertiesWidget->resetSettings();
}

void MainWindow::on_actionImage_Viewer_always_on_top_triggered(bool checked)
{
    if(checked)
    {
#ifdef Q_OS_WIN
        _imageViewer->setParent(this, Qt::Dialog);
#else
        _imageViewer->setParent(this, Qt::Dialog | Qt::WindowStaysOnTopHint);
#endif
    }
    else
    {
        _imageViewer->setParent(NULL, Qt::Window);
    }

    _imageViewer->show();

}

void MainWindow::on_actionCheck_for_updates_triggered()
{
#ifdef USE_FERVOR_UPDATER
    FvUpdater::sharedUpdater()->CheckForUpdatesNotSilent();
#endif
}
