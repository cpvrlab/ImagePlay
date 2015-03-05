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
