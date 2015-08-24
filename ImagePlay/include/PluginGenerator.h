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

#ifndef PLUGINGENERATOR_H
#define PLUGINGENERATOR_H

#include <QDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QDesktopServices>

#include <QDebug>

namespace Ui {
class PluginGenerator;
}

class PluginGenerator : public QDialog
{
    Q_OBJECT

public:
    explicit PluginGenerator(QWidget *parent, QString pluginDevPath, QString pluginPath, QString rootPath);
    ~PluginGenerator();

private slots:
    void on_btnGenerate_clicked();
    void on_btnCancel_clicked();

private:
    Ui::PluginGenerator *ui;

    QString _pluginDevPath;
    QString _pluginPath;
    QString _rootPath;
    QString _className;
    QString _title;
    QString _description;
    QString _keywords;
    QString _category;
    QString _openCV;
    QString _author;

    bool generateFiles();
    bool generateFile(QString inputName, QString outputName, QString folder);
};

#endif // PLUGINGENERATOR_H
