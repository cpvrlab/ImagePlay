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

#include "PluginGenerator.h"
#include "ui_PluginGenerator.h"
#include <QUrl>


PluginGenerator::PluginGenerator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PluginGenerator)
{
    ui->setupUi(this);

    // only allow alphanumeric input
    ui->inputClassName->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]*")));

    // populate comboboxes
    QStringList categoryNames;
    categoryNames.push_back("CATEGORY_UNDEFINED");
    categoryNames.push_back("CATEGORY_IO");
    categoryNames.push_back("CATEGORY_CONVERSIONS");
    categoryNames.push_back("CATEGORY_ARITHMETIC");
    categoryNames.push_back("CATEGORY_GEOMETRY");
    categoryNames.push_back("CATEGORY_POINTOPERATIONS");
    categoryNames.push_back("CATEGORY_LOCALOPERATIONS");
    categoryNames.push_back("CATEGORY_MORPHOLOGY");
    categoryNames.push_back("CATEGORY_HISTOGRAM");
    categoryNames.push_back("CATEGORY_EDGES");
    categoryNames.push_back("CATEGORY_GRADIENTS");
    categoryNames.push_back("CATEGORY_FOURIER");
    categoryNames.push_back("CATEGORY_OBJECTS");
    ui->comboBoxCategory->addItems(categoryNames);

    QStringList openCVNames;
    openCVNames.push_back("OPENCV_NONE");
    openCVNames.push_back("OPENCV_OPTIONAL");
    openCVNames.push_back("OPENCV_ONLY");
    ui->comboBoxOpenCV->addItems(openCVNames);

    _basePath = QDir::currentPath() + "/plugin_development/";
}

PluginGenerator::~PluginGenerator()
{
    delete ui;
}

void PluginGenerator::on_btnGenerate_clicked()
{
    // check form inputs
    if(ui->inputAuthor->text().length() < 4)
    {
        QMessageBox::information(this, "Form Error", "Please enter a valid Author name");
        ui->inputAuthor->setFocus();
        return;
    }
    if(ui->inputClassName->text().length() < 4
        || ui->inputClassName->text().at(0).isDigit()
        || ui->inputClassName->text().at(0).isLower())
    {
        QMessageBox::information(this, "Form Error", "Please enter a valid ClassName");
        ui->inputClassName->setFocus();
        return;
    }
    else if(ui->inputTitle->text().length() < 4)
    {
        QMessageBox::information(this, "Form Error", "Please enter a reasonable Title");
        ui->inputTitle->setFocus();
        return;
    }

    // get values
    _className = ui->inputClassName->text();
    _title = ui->inputClassName->text();
    _description = ui->inputDescription->text();
    _keywords = ui->inputClassName->text();
    _author = ui->inputClassName->text();
    _category = ui->comboBoxCategory->currentText();
    _openCV = ui->comboBoxOpenCV->currentText();

    // generate files
    if(generateFiles())
    {
        QMessageBox::information(this, "Success", "Your plugin was created successfully.");

        QUrl url = QUrl::fromLocalFile(_basePath);
        qDebug() << url.toString();
        QDesktopServices::openUrl(url);

        close();
    }
    else
    {
        QMessageBox::information(this, "Error", "Your plugin could not be created. Enable the log window (View -> Show Log) in order to see error messages.");
    }
}

void PluginGenerator::on_btnCancel_clicked()
{
    close();
}

bool PluginGenerator::generateFiles()
{
    // create folder
    QDir dir(_basePath);
    qWarning() << "Creating directory: " << dir.absoluteFilePath(_className);
    if(!dir.mkdir(_className))
    {
        qWarning() << "Couldn't create plugin folder.";
        return false;
    }

    // copy files and replace placeholders
    bool success = true;
    success &= generateFile("NAME.h", _className + ".h", _className);
    success &= generateFile("NAME.pro", _className + ".pro", _className);
    success &= generateFile("NAME.vcxproj", _className + ".vcxproj", _className);

    return success;
}

bool PluginGenerator::generateFile(QString inputName, QString outputName, QString folder)
{
    QFile fileTemplate(_basePath + "_template/" + inputName);
    if (!fileTemplate.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << fileTemplate.fileName() << " not readable!";
        return false;
    }

    QFile fileResult(_basePath + folder + "/" + outputName);
    if (!fileResult.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << fileResult.fileName() << " not writable!";
        return false;
    }

    QTextStream in(&fileTemplate);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        line = line.replace("%CLASSNAME%", _className);
        line = line.replace("%TITLE%", _title);
        line = line.replace("%DESCRIPTION%", _description);
        line = line.replace("%KEYWORDS%", _keywords);
        line = line.replace("%AUTHOR%", _author);
        line = line.replace("%CATEGORY%", _category);
        line = line.replace("%OPENCV%", _openCV);

        fileResult.write(line.append("\n").toLocal8Bit());
    }

    return true;
}
