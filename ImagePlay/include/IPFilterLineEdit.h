//#############################################################################
//  File:      IPFilterLineEdit.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPFILTERLINEEDIT_H
#define IPFILTERLINEEDIT_H

#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

//-----------------------------------------------------------------------------
//!Custom QLineEdit which contains a small reset button
class IPFilterLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit IPFilterLineEdit(QWidget *parent = 0);

signals:

public slots:
    void resetPressed();
    void resetReleased();
    void resetClicked();

private:
    QPushButton* _resetButton;

};

#endif // IPFILTERLINEEDIT_H
