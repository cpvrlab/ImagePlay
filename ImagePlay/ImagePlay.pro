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

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImagePlay
TEMPLATE = app
#TEMPLATE = vcapp

VERSION = "6.0.0-rc.2"
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
APP_NAME=\\\"$$TARGET\\\" \

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

HEADERS     += $$files(*.h,true)
SOURCES     += $$files(*.cpp,true)
FORMS       += $$files(*.ui,true)
RESOURCES   += $$files(*.qrc,true)
OTHER_FILES += $$files(*,true)

# exclude plugin templates
HEADERS     -= media/plugin_development/_template/NAME.h

RC_FILE = res/icon.rc


# ImagePlay Library
win32: {
    CONFIG(release, debug|release): LIBS += -L$$PWD/../_bin/$$CONFIGURATION/$$PLATFORM -lIPL
    else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../_bin/$$CONFIGURATION/$$PLATFORM -lIPL

    #copy stuff after compiling
    #resources.path = ../_bin/$$CONFIGURATION/$$PLATFORM
    #resources.files += media/process_icons
    #INSTALLS += resources

    #run windeployqt
    #copy dlls
    #copy media
    QMAKE_POST_LINK +=  windeployqt.exe --no-angle --no-svg --no-system-d3d-compiler --no-quick-import --no-translations ../_bin/$$CONFIGURATION/$$PLATFORM/ImagePlay.exe & \
                        $${QMAKE_COPY_DIR} ..\\_lib\\windows ..\\_bin\\$$CONFIGURATION\\$$PLATFORM\\ & \
                        $${QMAKE_COPY_DIR} media\\process_icons ..\\_bin\\$$CONFIGURATION\\$$PLATFORM\\process_icons\\ & \
                        $${QMAKE_COPY_DIR} media\\images ..\\_bin\\$$CONFIGURATION\\$$PLATFORM\\images\\ & \
                        $${QMAKE_COPY_DIR} media\\plugin_development ..\\_bin\\$$CONFIGURATION\\$$PLATFORM\\plugin_development & \
                        $${QMAKE_COPY_DIR} ..\\IPL\\include ..\\_bin\\$$CONFIGURATION\\$$PLATFORM\\plugin_development\\_lib\\include & \
                        del ..\\_bin\\$$CONFIGURATION\\$$PLATFORM\\IPL.exp & \
#                        del ..\\_bin\\$$CONFIGURATION\\$$PLATFORM\\IPL.lib & \
}

macx: {
    QMAKE_MAC_SDK = macosx10.11
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
    LIBS += -L../_bin/$$CONFIGURATION/$$PLATFORM/ -lIPL

    LIBS += -lfreeimage
    LIBS += -lopencv_core
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_highgui

    QMAKE_POST_LINK +=  $${QMAKE_COPY_DIR} media/process_icons/ ../_bin/$$CONFIGURATION/$$PLATFORM/ && \
                        $${QMAKE_COPY_DIR} media/images/ ../_bin/$$CONFIGURATION/$$PLATFORM/ &&\
                        $${QMAKE_MKDIR} ../_bin/$$CONFIGURATION/$$PLATFORM/plugin_development && \
                        rm -rf ../_bin/$$CONFIGURATION/$$PLATFORM/plugin_development/_lib ../_bin/$$CONFIGURATION/$$PLATFORM/plugin_development/_template && \
                        $${QMAKE_COPY_DIR} media/plugin_development/_template ../_bin/$$CONFIGURATION/$$PLATFORM/plugin_development && \
                        $${QMAKE_MKDIR} ../_bin/$$CONFIGURATION/$$PLATFORM/plugin_development/_lib && \
                        $${QMAKE_COPY_DIR} ../IPL/include ../_bin/$$CONFIGURATION/$$PLATFORM/plugin_development/_lib/include \
}

unix : !macx : !isEqual(QMAKE_WIN32,1){
        isEmpty(PREFIX): PREFIX = /usr
        !isEqual(AUTOTYPE,0){
                DEFINES += AUTOTYPE
                !isEqual(GLOBAL_AUTOTYPE,0){
                        DEFINES += GLOBAL_AUTOTYPE
                }
        }
        TARGET = imageplay
        target.path = $${PREFIX}/bin
        images.files = ../_bin/$$CONFIGURATION/$$PLATFORM/images/*
        images.path = $${PREFIX}/share/imageplay/images
        process_icons.files = ../_bin/$$CONFIGURATION/$$PLATFORM/process_icons/*.png
        process_icons.path = $${PREFIX}/share/imageplay/process_icons
        plugin_development.files = ../_bin/$$CONFIGURATION/$$PLATFORM/plugin_development/*
        plugin_development.path = $${PREFIX}/share/imageplay/plugin_development

        INSTALLS += images process_icons plugin_development
        INSTALLS += target

        DEFINES += LINUX_PATH_PREFIX=\"\\\"$$PREFIX/share/imageplay/\\\"\"
}

msvc {
    #QMAKE_CXXFLAGS_RELEASE -= -O1
    #QMAKE_CXXFLAGS_RELEASE -= -O2
    #QMAKE_CXXFLAGS_RELEASE *= -O3
}

clang {
    CONFIG +=c++11
}

gcc:!clang {
    CONFIG +=c++11
    LIBS += -lgomp
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
