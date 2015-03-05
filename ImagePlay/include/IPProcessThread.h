//#############################################################################
//  File:      IPProcessThread.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPPROCESSTHREAD_H
#define IPPROCESSTHREAD_H

#include <QThread>

#include "IPL_processes.h"

#include <QDebug>

//-----------------------------------------------------------------------------
//!IPProcessThread executes the process algorithm
/*!
 * References IPLProcess which contains the logic.
 */

class IPProcessThread : public QThread, public IPLProgressEventHandler
{
    Q_OBJECT
public:
                            IPProcessThread         (IPLProcess* process, IPLImage* image = NULL, int inputIndex = 0, bool useOpenCV = false);
                            ~IPProcessThread        ();
    void                    run                     ();
    void                    updateProgress          (int percent);
    IPLProcess*             process                 ()                  { return _process; }
    bool                    success                 ()                  { return _success; }

signals:
    void progressUpdated(int);

private:
    int                     _inputIndex;
    IPLProcess*             _process;
    IPLImage*               _image;
    bool                    _success;
    bool                    _useOpenCV;
};


#endif // IPPROCESSTHREAD_H
