#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"

#include "MainWindow.h"

SettingsWindow::SettingsWindow(MainWindow *mainWindow) :
    QDialog((QWidget*)mainWindow),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    _mainWindow = mainWindow;
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_buttonBox_accepted()
{
    // save the settings
    _mainWindow->setDefaultImagePath(ui->txtDefaultImagePath->text());
    _mainWindow->setLogFileEnabled(ui->chkLogFile->isChecked());
    _mainWindow->setUseOpenCV(ui->chkOpenCV->isChecked());
    _mainWindow->setAutosaveEnabled(ui->chkAutosave->isChecked());

    _mainWindow->writeSettings();

    close();
}

void SettingsWindow::on_buttonBox_rejected()
{
    close();
}


void SettingsWindow::showEvent(QShowEvent *)
{
    // display the current settings
    ui->txtDefaultImagePath->setText(_mainWindow->defaultImagePath());
    ui->chkAutosave->setChecked(_mainWindow->autosaveEnabled());
    ui->chkOpenCV->setChecked(_mainWindow->useOpenCV());
    ui->chkLogFile->setChecked(_mainWindow->logFileEnabled());

    updatePluginList();
}

void SettingsWindow::on_btnDefaultImagePath_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), "");

    if(folder.length() > 0)
    {
        ui->txtDefaultImagePath->setText(folder);
    }
}

void SettingsWindow::on_btnReloadPlugins_clicked()
{
    _mainWindow->reloadPlugins();
    updatePluginList();
}

void SettingsWindow::updatePluginList()
{
    ui->listWidget->clear();

    if (_mainWindow->loadedPlugins()->count() > 0)
    {
        for (int i = 0; i < _mainWindow->loadedPlugins()->count(); i++)
        {
            PluginInterface* plugin = _mainWindow->loadedPlugins()->at(i);
            QString pluginInfo("%1\tVersion: %2\tAuthor : %3");
            ui->listWidget->addItem(pluginInfo.arg(plugin->name()).arg(plugin->version()).arg(plugin->author()));
        }
    }
    else
        ui->listWidget->addItem("No plugins loaded");
}

void SettingsWindow::on_btnUnloadPlugins_clicked()
{
    _mainWindow->unloadPlugins();
    updatePluginList();
}
