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
