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

#ifndef IPPROCESSPROPERTIESWIDGET_H
#define IPPROCESSPROPERTIESWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QSpinBox>
#include <QLineEdit>
#include "PropertyWidgets/IPPropertyButtonInt.h"
#include "PropertyWidgets/IPPropertySliderInt.h"
#include "PropertyWidgets/IPPropertySliderIntOdd.h"
#include "PropertyWidgets/IPPropertySliderIntEven.h"
#include "PropertyWidgets/IPPropertySpinnerInt.h"
#include "PropertyWidgets/IPPropertySliderDouble.h"
#include "PropertyWidgets/IPPropertyFileOpen.h"
#include "PropertyWidgets/IPPropertyFileSave.h"
#include "PropertyWidgets/IPPropertyColorRGB.h"
#include "PropertyWidgets/IPPropertyColorHSL.h"
#include "PropertyWidgets/IPPropertyColorHSV.h"
#include "PropertyWidgets/IPPropertyCheckbox.h"
#include "PropertyWidgets/IPPropertyKernelInt.h"
#include "PropertyWidgets/IPPropertyRadioInt.h"
#include "PropertyWidgets/IPPropertyCheckboxInt.h"
#include "PropertyWidgets/IPPropertyBinaryMorphologyInt.h"
#include "PropertyWidgets/IPPropertyBinaryMorphologyTristateInt.h"
#include "PropertyWidgets/IPPropertyGrayscaleMorphologyInt.h"
#include "PropertyWidgets/IPPropertyString.h"
#include "PropertyWidgets/IPPropertyFolder.h"
#include "PropertyWidgets/IPPropertyCombobox.h"
#include "PropertyWidgets/IPPropertyGroup.h"
#include "PropertyWidgets/IPPropertyPoint.h"
#include "PropertyWidgets/IPPropertyMatrixDouble.h"
#include "MainWindow.h"

#include <QDebug>

#include "IPL_processes.h"

//-----------------------------------------------------------------------------
//!IPProcessPropertiesWidget displays settings for IPLProcess
/*!
 * Displays GUI controls for all types of process properties.
 * The GUI and layout are built dynamically and destroyed when hidden.
 */
class IPProcessPropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit                    IPProcessPropertiesWidget   (QWidget *parent = 0);
                                ~IPProcessPropertiesWidget  ();
    void                        init                        (IPProcessStep* process);
    void                        closeSettings               ();
    void                        addPropertyWidget           (QString label, QString description, IPPropertyWidget *widget);
    void                        setMainWindow               (MainWindow* mainWindow)                                            { _mainWindow = mainWindow; }
    MainWindow*                 mainWindow                  ();

private:
    static bool                 sortByPosition              (IPLProcessProperty* x, IPLProcessProperty* y);
    IPProcessStep*              _processStep;
    QList<IPPropertyWidget*>    _propertyWidgets;
    MainWindow*                 _mainWindow;

public slots:
    void                        showPropertyGroup           (QString);
};

#endif // IPPROCESSPROPERTIESWIDGET_H
