#include "AboutWindow.h"
#include "ui_AboutWindow.h"

#include "MainWindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);

    QString version("<b>Version: </b> %1, <b>Build: </b> %2");

    ui->labelVersion->setText(version.arg(IMAGEPLAY_VERSION).arg(IMAGEPLAY_BUILDNUMBER));
}

AboutWindow::~AboutWindow()
{
    delete ui;
}
