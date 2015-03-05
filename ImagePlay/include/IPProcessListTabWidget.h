//#############################################################################
//  File:      IPProcessListTabWidget.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPPROCESSLISTTABWIDGET_H
#define IPPROCESSLISTTABWIDGET_H

#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>

#include "IPProcessList.h"
#include "MainWindow.h"

//-----------------------------------------------------------------------------
//!Custom QTabWidget which contains process lists for all categories
/*!
 *
 */
class IPProcessListTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit        IPProcessListTabWidget  (QWidget *parent = 0);
    void            init                    (MainWindow* mainWindow);
    void            filter                  (QString text);

private:
    IPProcessList*  _allProcessesList;
};

#endif // IPPROCESSLISTTABWIDGET_H
