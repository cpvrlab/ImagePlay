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

#include "TutorialDialog.h"
#include "ui_TutorialDialog.h"

TutorialDialog::TutorialDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TutorialDialog)
{
    ui->setupUi(this);

    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    _currentStep = 0;
    _maxSteps = 4;

    // button colors
    QPalette buttonBlue = ui->pushButton->palette();
    buttonBlue.setColor(QPalette::Button, QColor(41, 128, 185));
    ui->pushButton->setPalette(buttonBlue);

    ui->pushButton->setDefault(true);
}

TutorialDialog::~TutorialDialog()
{
    delete ui;
}

void TutorialDialog::on_pushButton_clicked()
{
    _currentStep++;

    // finished
    if(_currentStep >= _maxSteps)
    {
        delete this;
        return;
    }

    QString title = QString("Tutorial (%1/%2)").arg(_currentStep+1).arg(_maxSteps);

    ui->label->setText(title);

    QPropertyAnimation *animation = new QPropertyAnimation(ui->tutorialContent, "geometry");
    int offset = -600*_currentStep;

    QRect pos =ui->tutorialContent->geometry();
    animation->setDuration(1000);
    animation->setStartValue(pos);
    pos.setLeft(offset);
    animation->setEndValue(pos);
    animation->setEasingCurve(QEasingCurve::InOutCubic);

    animation->start();

    // last step
    if(_currentStep == _maxSteps-1)
    {
        // button colors
        QPalette buttonGreen = ui->pushButton->palette();
        buttonGreen.setColor(QPalette::Button, QColor(0, 150, 0));
        ui->pushButton->setPalette(buttonGreen);

        ui->pushButton->setText("Finish");
    }
}

void TutorialDialog::on_pushButton_2_clicked()
{
    delete this;
}
