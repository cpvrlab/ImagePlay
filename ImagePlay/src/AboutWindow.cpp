#include "AboutWindow.h"
#include "ui_AboutWindow.h"

#include "MainWindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);

    QString version("<b>Version: </b>");
    version.append(IMAGEPLAY_VERSION);

    ui->labelVersion->setText(version);
}

AboutWindow::~AboutWindow()
{
    delete ui;
}
