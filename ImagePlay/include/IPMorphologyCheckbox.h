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
