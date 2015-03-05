//#############################################################################
//  File:      SettingsWindow.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class SettingsWindow;
}

class MainWindow;

//-----------------------------------------------------------------------------
//!SettingsWindow
/*!
 *
 */
class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit            SettingsWindow                  (MainWindow *parent = 0);
                        ~SettingsWindow                 ();
private slots:
    void                on_buttonBox_accepted           ();
    void                on_buttonBox_rejected           ();
    void                on_btnDefaultImagePath_clicked  ();
    void                on_btnReloadPlugins_clicked     ();
    void                on_btnUnloadPlugins_clicked     ();

private:
    Ui::SettingsWindow* ui;
    MainWindow*         _mainWindow;
    void                updatePluginList                ();

    // QWidget interface
protected:
    void                showEvent                       (QShowEvent *);
};

#endif // propertiesWINDOW_H
