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

