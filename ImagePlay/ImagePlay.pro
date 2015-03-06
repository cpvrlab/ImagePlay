##############################################################################
#  File:      ImagePlay.pro
#  Purpose:   QMake project definition file for ImagePlay Application
#  Author:    Kaspar Schmid
#  Date:      December 2014
#  Copyright: Kaspar Schmid, Switzerland
#             THIS SOFTWARE IS PROVIDED FOR EDUCATIONAL PURPOSE ONLY AND
#             WITHOUT ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED.
##############################################################################

QT       += core gui

PRECOMPILED_HEADER = include/stafx.h

QMAKE_CXXFLAGS+= -openmp

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImagePlay
TEMPLATE = app
#TEMPLATE = vcapp

VERSION = 6.0.0-beta
QMAKE_TARGET_COMPANY = "BFH"
QMAKE_TARGET_PRODUCT = "ImagePlay"
QMAKE_TARGET_DESCRIPTION = ""
QMAKE_TARGET_COPYRIGHT = ""

DEFINES += \
APP_VERSION=\"\\\"$$VERSION\\\"\" \
APP_COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\" \
APP_PRODUCT=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\" \
APP_DESCRIPTION=\"\\\"$$QMAKE_TARGET_DESCRIPTION\\\"\" \
APP_COPYRIGHT=\"\\\"$$QMAKE_TARGET_COPYRIGHT\\\"\" \
APP_NAME=\\\"$$TARGET\\\"


#define platform variable for folder name
win32 {contains(QMAKE_TARGET.arch, x86_64) {PLATFORM = x64} else {PLATFORM = Win32}}
macx {PLATFORM = macx}
unix:!macx:!android {PLATFORM = linux}

#define configuration variable for folder name
CONFIG(debug, debug|release) {CONFIGURATION = Debug} else {CONFIGURATION = Release}

DESTDIR = ../_bin/$$CONFIGURATION/$$PLATFORM
OBJECTS_DIR = ../intermediate/$$TARGET/$$CONFIGURATION/$$PLATFORM
MOC_DIR = ../intermediate/$$TARGET/$$CONFIGURATION/$$PLATFORM
RCC_DIR = ../intermediate/$$TARGET/$$CONFIGURATION/$$PLATFORM
UI_DIR = ../intermediate/$$TARGET/$$CONFIGURATION/$$PLATFORM


SOURCES += src/main.cpp\
    src/IPApplication.cpp \
    src/IPHistogramWidget.cpp \
    src/ImageViewerWindow.cpp \
    src/IPProcessListTabWidget.cpp \
    src/IPProcessMessageWidget.cpp \
    src/IPFilterLineEdit.cpp \
    src/IPMorphologyCheckbox.cpp \
    src/IPProcessStep.cpp \
    src/IPProcessEdge.cpp \
    src/IPProcessEdgeTmp.cpp \
    src/IPProcessGrid.cpp \
    src/IPProcessGridScene.cpp \
    src/IPProcessList.cpp \
    src/IPProcessFactory.cpp \
    src/IPProcessScript.cpp \
    src/IPImageViewer.cpp \
    src/IPProcessPropertiesWidget.cpp \
    src/ConnectionDialog.cpp \
    src/SettingsWindow.cpp \
    src/AboutWindow.cpp \
    src/MainWindow.cpp \
    src/TutorialDialog.cpp \
    src/IPZoomWidget.cpp \
    src/IPProcessThread.cpp \
    src/PluginGenerator.cpp

HEADERS  += \
    include/IPApplication.h \
    include/IPProcessPropertiesWidget.h \
    include/IPProcessMessageWidget.h \
    include/PropertyWidgets/IPPropertyCheckbox.h \
    include/PropertyWidgets/IPPropertyColorHSL.h \
    include/PropertyWidgets/IPPropertyColorHSV.h \
    include/PropertyWidgets/IPPropertyColorRGB.h \
    include/PropertyWidgets/IPPropertySliderInt.h \
    include/PropertyWidgets/IPPropertySpinnerInt.h \
    include/PropertyWidgets/IPPropertyWidget.h \
    include/PropertyWidgets/IPPropertySliderDouble.h \
    include/PropertyWidgets/IPPropertyKernelInt.h \
    include/PropertyWidgets/IPPropertyRadioInt.h \
    include/PropertyWidgets/IPPropertyCheckboxInt.h \
    include/PropertyWidgets/IPPropertyBinaryMorphologyInt.h \
    include/PropertyWidgets/IPPropertyGrayscaleMorphologyInt.h \
    include/PropertyWidgets/IPPropertyBinaryMorphologyTristateInt.h \
    include/PropertyWidgets/IPKernelPreset.h \
    include/PropertyWidgets/IPPropertyString.h \
    include/PropertyWidgets/IPPropertyPoint.h \
    include/PropertyWidgets/IPPropertyFileSave.h \
    include/PropertyWidgets/IPPropertyFileOpen.h \
    include/PropertyWidgets/IPPropertyFolder.h \
    include/PropertyWidgets/IPPropertyCombobox.h \
    include/IPProcessListTabWidget.h \
    include/IPMorphologyCheckbox.h \
    include/IPProcessThread.h \
    include/IPHistogramThread.h \
    include/IPHistogramWidget.h \
    include/IPZoomWidget.h \
    include/ImageViewerWindow.h \
    include/ConnectionDialog.h \
    include/IPFilterLineEdit.h \
    include/IPProcessStep.h \
    include/IPProcessEdge.h \
    include/IPProcessEdgeTmp.h \
    include/IPProcessGrid.h \
    include/IPProcessGridScene.h \
    include/IPProcessList.h \
    include/IPProcessFactory.h \
    include/IPProcessScript.h \
    include/IPImageViewer.h \
    include/SettingsWindow.h \
    include/AboutWindow.h \
    include/TutorialDialog.h \
    include/MainWindow.h \
    include/PluginGenerator.h \
    include/stafx.h \

FORMS    += \
    ui/ImageViewerWindow.ui \
    ui/SettingsWindow.ui \
    ui/AboutWindow.ui \
    ui/MainWindow.ui \
    ui/TutorialDialog.ui \
    ui/PluginGenerator.ui

RESOURCES += res/ressources.qrc

RC_FILE = res/icon.rc


# ImagePlay Library
win32: {
    CONFIG(release, debug|release): LIBS += -L$$PWD/../_bin/$$CONFIGURATION/$$PLATFORM -lIPL
    else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../_bin/$$CONFIGURATION/$$PLATFORM -lIPL

    #copy stuff after compiling
    resources.path = ../_bin/$$CONFIGURATION/$$PLATFORM
    resources.files += ../_res/*
    resources.files += ../_res/platforms
    resources.files += ../_res/process_icons
    INSTALLS += resources
}

macx: {
    CONFIG +=c++11
    LIBS += -L../_bin/$$CONFIGURATION/$$PLATFORM/ImagePlay.app/Contents/Frameworks/ -lIPL

    mylib.path = Contents/Frameworks
    mylib.files = \
      ../_lib/opencv/x64/clang/lib/libopencv_core.2.4.10.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_highgui.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_core.2.4.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_imgproc.2.4.10.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_core.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_imgproc.2.4.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_highgui.2.4.10.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_imgproc.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_highgui.2.4.dylib \
      ../_lib/freeimage/libfreeimage-3.16.0.dylib-x86_64
        
    QMAKE_BUNDLE_DATA += mylib

    #copy stuff after compiling
    resources.path = Contents/MacOS
    resources.files += media/process_icons
    QMAKE_BUNDLE_DATA += resources

    ICON = res/ImagePlay.icns

    #run macdeployqt
    QMAKE_POST_LINK += macdeployqt ../_bin/$$CONFIGURATION/$$PLATFORM/ImagePlay.app/ -dmg

}

linux: {
    CONFIG += c++11

    LIBS += -L../_bin/$$CONFIGURATION/$$PLATFORM/ -lIPL

    LIBS += -lfreeimage
    LIBS += -lopencv_core
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_highgui

    resources.path = ../_bin/$$CONFIGURATION/$$PLATFORM/
    resources.files += media/process_icons
    QMAKE_BUNDLE_DATA += resources

}


INCLUDEPATH += $$PWD/include/
INCLUDEPATH += $$PWD/../IPL/include/
INCLUDEPATH += $$PWD/../IPL/include/processes/
INCLUDEPATH += $$PWD/../IPL/include/opencv/
DEPENDPATH += $$PWD/../IPL/include/

# Visual Leak Detector
#win32: LIBS += -L"C:/Program Files (x86)/Visual Leak Detector/lib/Win32/" -lvld

#INCLUDEPATH += "C:/Program Files (x86)/Visual Leak Detector/include/"
#DEPENDPATH += "C:/Program Files (x86)/Visual Leak Detector/include/"


#INCLUDEPATH += $$PWD/../_bin/Release/macx
#DEPENDPATH += $$PWD/../_bin/Release/macx
