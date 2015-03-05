//#############################################################################
//  File:      IPApplication.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPAPPLICATION_H
#define IPAPPLICATION_H

#include <QApplication>
#include <QMessageBox>

//-----------------------------------------------------------------------------
//!Custom IPApplication used to override notify, currently unused
class IPApplication : public QApplication
{
public:
    IPApplication(int &argc, char **argv);
public:
//    bool notify(QObject *, QEvent *);
};

#endif // IPAPPLICATION_H
