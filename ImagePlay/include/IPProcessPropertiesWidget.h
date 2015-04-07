//#############################################################################
//  File:      IPProcessPropertiesWidget.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
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
#include "PropertyWidgets/IPPropertyPoint.h"
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
    void                        init                        (IPProcessStep* process);
    void                        closeSettings               ();
    void                        addPropertyWidget           (QString label, QString description, IPPropertyWidget *widget);
    void                        setMainWindow               (MainWindow* mainWindow)                                            { _mainWindow = mainWindow; }
    MainWindow*                 mainWindow                  ()                                                                  { return _mainWindow; }

private:
    static bool                 sortByPosition              (IPLProcessProperty* x, IPLProcessProperty* y);
    IPProcessStep*              _processStep;
    QList<IPPropertyWidget*>    _propertyWidgets;
    MainWindow*                 _mainWindow;
};

#endif // IPPROCESSPROPERTIESWIDGET_H
