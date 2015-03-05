//#############################################################################
//  File:      TutorialDialog.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
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
