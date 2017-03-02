#############################################################################
#
#  This file is part of ImagePlay.
#
#  ImagePlay is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  ImagePlay is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with ImagePlay.  If not, see <http://www.gnu.org/licenses/>.
#
##############################################################################

QT       += core gui

PRECOMPILED_HEADER = include/stafx.h

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImagePlay
TEMPLATE = app
#TEMPLATE = vcapp

VERSION = "6.1.0"
QMAKE_TARGET_COMPANY = "BFH"
QMAKE_TARGET_PRODUCT = "ImagePlay"
QMAKE_TARGET_DESCRIPTION = ""
QMAKE_TARGET_COPYRIGHT = ""

equals(TEMPLATE, app) {
    #message("Template value equals app")
    DEFINES += \
    APP_VERSION=\"\\\"$$VERSION\\\"\" \
    APP_COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\" \
    APP_PRODUCT=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\" \
    APP_DESCRIPTION=\"\\\"$$QMAKE_TARGET_DESCRIPTION\\\"\" \
    APP_COPYRIGHT=\"\\\"$$QMAKE_TARGET_COPYRIGHT\\\"\" \
    APP_NAME=\\\"$$TARGET\\\" \

} else {
    #message("Template value equals vcapp")
    DEFINES += \
    APP_VERSION=\\\"$$VERSION\\\" \
    APP_COMPANY=\\\"$$QMAKE_TARGET_COMPANY\\\" \
    APP_PRODUCT=\\\"$$QMAKE_TARGET_PRODUCT\\\" \
    APP_DESCRIPTION=\\\"$$QMAKE_TARGET_DESCRIPTION\\\" \
    APP_COPYRIGHT=\\\"$$QMAKE_TARGET_COPYRIGHT\\\" \
    APP_NAME=\\\"$$TARGET\\\" \
}

# enable or disable update checker
USE_FERVOR_UPDATER = false
DEFINES += IMAGEPLAY_URL=\\\"http://imageplay.io\\\"
DEFINES += IMAGEPLAY_APPCAST_URL=\\\"http://imageplay.io/Appcast.xml\\\"

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
SOURCES     -= media/plugin_development/_template/NAME.cpp
SOURCES     -= media/plugin_development/_template/plugin.cpp


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
                        $${QMAKE_COPY_DIR} media\\examples ..\\_bin\\$$CONFIGURATION\\$$PLATFORM\\examples\\ & \
                        $${QMAKE_COPY_DIR} media\\examples\images ..\\_bin\\$$CONFIGURATION\\$$PLATFORM\\examples\\images\\ & \
                        $${QMAKE_COPY_DIR} media\\plugin_development ..\\_bin\\$$CONFIGURATION\\$$PLATFORM\\plugin_development & \
                        $${QMAKE_COPY_DIR} ..\\IPL\\include ..\\_bin\\$$CONFIGURATION\\$$PLATFORM\\plugin_development\\_lib\\include & \
                        del ..\\_bin\\$$CONFIGURATION\\$$PLATFORM\\IPL.exp & \
#                        del ..\\_bin\\$$CONFIGURATION\\$$PLATFORM\\IPL.lib & \

    USE_FERVOR_UPDATER = true
}

macx: {
    QMAKE_MAC_SDK = macosx10.12
    QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter

    #LIBS += -L$$PWD/../_lib/freeimage/ -lfreeimage-3.16.0
    #LIBS += -L../_bin/$$CONFIGURATION/$$PLATFORM/ImagePlay.app/Contents/Frameworks/ -lIPL
    LIBS += -L$$PWD/../_lib/ -lIPL
    LIBS += -L$$PWD/../_lib/freeimage/ -lfreeimage-3.16.0

    #INCLUDEPATH += $$PWD/../_lib/freeimage
    #DEPENDPATH += $$PWD/../_lib/freeimage

    mylib.path = Contents/Frameworks
    mylib.files = \
      ../_lib/opencv/x64/clang/lib/libopencv_core.3.1.0.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_imgproc.3.1.0.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_highgui.3.1.0.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_videoio.3.1.0.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_calib3d.3.1.0.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_optflow.3.1.0.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_features2d.3.1.0.dylib \
      ../_lib/opencv/x64/clang/lib/libopencv_xfeatures2d.3.1.0.dylib \
      ../_lib/libIPL.1.0.0.dylib \
      ../_lib/freeimage/libfreeimage-3.16.0.dylib-x86_64 \
        
    QMAKE_BUNDLE_DATA += mylib

    #copy stuff after compiling
    resources.path = Contents/MacOS
    resources.files += media/process_icons
    QMAKE_BUNDLE_DATA += resources

    ICON = res/ImagePlay.icns

    #run macdeployqt
    #QMAKE_POST_LINK += macdeployqt ../_bin/$$CONFIGURATION/$$PLATFORM/ImagePlay.app/ -dmg
    QMAKE_POST_LINK += macdeployqt ../_bin/$$CONFIGURATION/$$PLATFORM/ImagePlay.app/ $$escape_expand(\n\t)

    # fix some weird dylib issues
    QMAKE_POST_LINK += install_name_tool -change libIPL.1.dylib @executable_path/../Frameworks/libIPL.1.0.0.dylib ../_bin/$$CONFIGURATION/$$PLATFORM/ImagePlay.app/Contents/MacOS/ImagePlay $$escape_expand(\n\t)
    QMAKE_POST_LINK += install_name_tool -change libfreeimage-3.16.0.dylib-x86_64 @executable_path/../Frameworks/libfreeimage-3.16.0.dylib-x86_64 ../_bin/$$CONFIGURATION/$$PLATFORM/ImagePlay.app/Contents/MacOS/ImagePlay $$escape_expand(\n\t)
    QMAKE_POST_LINK += install_name_tool -change libfreeimage-3.16.0.dylib-x86_64 @executable_path/../Frameworks/libfreeimage-3.16.0.dylib-x86_64 ../_bin/$$CONFIGURATION/$$PLATFORM/ImagePlay.app/Contents/Frameworks/libIPL.1.0.0.dylib $$escape_expand(\n\t)

    USE_FERVOR_UPDATER = true
}

linux: {
    LIBS += -L../_bin/$$CONFIGURATION/$$PLATFORM/ -lIPL

    LIBS += -lfreeimage
    LIBS += -lopencv_core
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_highgui
    LIBS += -lopencv_videoio
    LIBS += -lopencv_calib3d
    LIBS += -lopencv_optflow
    LIBS += -lopencv_features2d
    LIBS += -lopencv_xfeatures2d
    LIBS += -lopencv_photo
    LIBS += -lopencv_xphoto
    LIBS += -ldl

    QMAKE_POST_LINK +=  $${QMAKE_COPY_DIR} media/process_icons/ ../_bin/$$CONFIGURATION/$$PLATFORM/ && \
                        $${QMAKE_COPY_DIR} media/examples/ ../_bin/$$CONFIGURATION/$$PLATFORM/ &&\
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

        INSTALLS += images
        INSTALLS += process_icons
        INSTALLS += plugin_development
        INSTALLS += target
        # LINUX_PATH_PREFIX is used later in MainWindow to identify location of process_icons, plugins and plugin_development
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

# Fervor autoupdater
if($$USE_FERVOR_UPDATER) {
    !include("../_fervor/Fervor.pri") {
        error("Unable to include Fervor autoupdater.")
    }
    DEFINES += USE_FERVOR_UPDATER
}

message($$DEFINES)

# Visual Leak Detector
#win32: LIBS += -L"C:/Program Files (x86)/Visual Leak Detector/lib/Win32/" -lvld

#INCLUDEPATH += "C:/Program Files (x86)/Visual Leak Detector/include/"
#DEPENDPATH += "C:/Program Files (x86)/Visual Leak Detector/include/"


#INCLUDEPATH += $$PWD/../_bin/Release/macx
#DEPENDPATH += $$PWD/../_bin/Release/macx
