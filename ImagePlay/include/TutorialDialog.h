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

#ifndef TUTORIALDIALOG_H
#define TUTORIALDIALOG_H

#include <QDialog>
#include <QPropertyAnimation>

namespace Ui {
class TutorialDialog;
}

//-----------------------------------------------------------------------------
//!TutorialDialog
/*!
 *
 */
class TutorialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit                TutorialDialog          (QWidget *parent = 0);
                            ~TutorialDialog         ();

private slots:
    void                    on_pushButton_clicked   ();
    void                    on_pushButton_2_clicked ();

private:
    Ui::TutorialDialog*     ui;
    int                     _currentStep;
    int                     _maxSteps;
};

#endif // TUTORIALDIALOG_H
