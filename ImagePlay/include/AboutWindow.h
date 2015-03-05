//#############################################################################
//  File:      AboutWindow.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>

namespace Ui {
class AboutWindow;
}

//-----------------------------------------------------------------------------
//! About window with copyright information
class AboutWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = 0);
    ~AboutWindow();

private:
    Ui::AboutWindow *ui;
};

#endif // ABOUTWINDOW_H
