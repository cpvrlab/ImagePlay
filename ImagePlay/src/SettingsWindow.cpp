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
    _mainWindow->setPluginDevPath(ui->txtPluginDevPath->text());
    _mainWindow->setPluginPath(ui->txtPluginPath->text());
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
    ui->txtPluginDevPath->setText(_mainWindow->pluginDevPath());
    ui->txtPluginPath->setText(_mainWindow->pluginPath());
    ui->chkAutosave->setChecked(_mainWindow->autosaveEnabled());
    ui->chkOpenCV->setChecked(_mainWindow->useOpenCV());
    ui->chkLogFile->setChecked(_mainWindow->logFileEnabled());

    updatePluginList();
}

void SettingsWindow::on_btnReloadPlugins_clicked()
{
    _mainWindow->reloadPlugins();
    updatePluginList();
}

void SettingsWindow::updatePluginList()
{
    ui->listWidget->clear();

    std::vector<IPLProcessDriver*>* drivers = _mainWindow->pluginManager()->loadedDrivers();

    if (drivers->size() > 0)
    {
        for (int i = 0; i < drivers->size(); i++)
        {
            IPLProcessDriver* driver = drivers->at(i);
            QString pluginInfo("%1\tVersion: %2\tAuthor : %3");
            ui->listWidget->addItem(pluginInfo.arg(QString::fromStdString(driver->className()))
                                              .arg(driver->version())
                                              .arg(QString::fromStdString(driver->author())));
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

void SettingsWindow::on_btnDefaultImagePath_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), "");

    if(folder.length() > 0)
    {
        ui->txtDefaultImagePath->setText(folder);
    }
}

void SettingsWindow::on_btnDevPath_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), "");

    if(folder.length() > 0)
    {
        ui->txtPluginDevPath->setText(folder);
    }
}


void SettingsWindow::on_btnPluginPath_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), "");

    if(folder.length() > 0)
    {
        ui->txtPluginPath->setText(folder);
    }
}
