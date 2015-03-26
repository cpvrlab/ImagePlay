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
    explicit PluginGenerator(QWidget *parent = 0);
    ~PluginGenerator();

private slots:
    void on_btnGenerate_clicked();
    void on_btnCancel_clicked();

private:
    Ui::PluginGenerator *ui;

    QString _basePath;
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
