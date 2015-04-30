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
