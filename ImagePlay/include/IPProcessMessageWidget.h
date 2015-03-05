//#############################################################################
//  File:      IPProcessMessageWidget.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPPROCESSERRORWIDGET_H
#define IPPROCESSERRORWIDGET_H

#include <QWidget>

#include "IPL_processes.h"
#include "MainWindow.h"

//-----------------------------------------------------------------------------
//!IPProcessMessageWidget displays errors and messages for IPLProcess
/*!
 *
 */
class IPProcessMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit                    IPProcessMessageWidget      (QWidget *parent = 0);
    void                        init                        (IPProcessStep* process);
    void                        updateMessages              ();

private:
    IPProcessStep*              _processStep;
    MainWindow*                 _mainWindow;
};

#endif // IPPROCESSERRORWIDGET_H

