//#############################################################################
//  File:      IPMorphologyCheckbox.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPMORPHOLOGYCHECKBOX_H
#define IPMORPHOLOGYCHECKBOX_H

#include <QCheckBox>
#include <QPaintEvent>
#include <QPainter>

#include <QDebug>

//-----------------------------------------------------------------------------
//!Custom QCheckBox which renders as black, white or gray square
class IPMorphologyCheckbox : public QCheckBox
{
    Q_OBJECT
public:
    explicit IPMorphologyCheckbox(QWidget *parent = 0);

signals:

public slots:


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *);
    void enterEvent(QEvent *);
};

#endif // IPMORPHOLOGYCHECKBOX_H
