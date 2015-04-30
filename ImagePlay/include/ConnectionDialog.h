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

#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QTimer>

#include "IPProcessStep.h"
#include "IPProcessGridScene.h"

class IPProcessGridScene;

//-----------------------------------------------------------------------------
//! Frameless dialog for connecting two processes
/*!
ConnectionDialog defines a GUI dialog which is shown whenever two processes
with multiple inputs or outputs are getting connected.
*/
class ConnectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectionDialog(IPProcessStep *from, IPProcessStep *to, IPProcessGridScene* scene, QWidget *parent);
signals:

public slots:
    void on_accept();
    void on_cancel();
    void redrawConnection();

private:
    IPProcessGridScene* _scene;     //!< reference to the QGraphicsScene
    IPProcessStep*      _from;      //!< First process to connect
    IPProcessStep*      _to;        //!< Second process to connect
    QButtonGroup*       _groupFrom; //!< First radiobutton group
    QButtonGroup*       _groupTo;   //!< Second radiobutton group
    QGridLayout*        _layout;    //!< QLayout
    QPushButton*        _btnAccept; //!< QPushbutton
    QPushButton*        _btnCancel; //!< QPushbutton
    MainWindow*         _mainWindow;//!< QPushbutton

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);
};

#endif // CONNECTIONDIALOG_H
