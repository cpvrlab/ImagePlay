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

CONFIG -= qt

TARGET = IPL
CONFIG(debug, debug|release): DESTDIR  = ../ImagePlay/debug
else: DESTDIR  = ../ImagePlay/release

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

#TEMPLATE = vclib
TEMPLATE = lib
#CONFIG += lib_bundle


DEFINES += IPL_LIBRARY

HEADERS += $$files(*.h,true)
SOURCES += $$files(*.cpp,true)
OTHER_FILES += $$files(*,true)

#win32: LIBS += -L$$PWD/lib/FreeImage/ -lFreeImage

#INCLUDEPATH += $$PWD/lib/FreeImage/
#DEPENDPATH += $$PWD/lib/FreeImage/

#win32: PRE_TARGETDEPS += $$PWD/lib/FreeImage/FreeImage.lib

win32 {
    # dirent
    INCLUDEPATH += $$PWD/lib/
    SOURCES += include/dirent/dirent.c
    HEADERS += include/dirent/dirent.h

    # freeimage
    LIBS += -L$$PWD/../_lib/freeimage/ -lFreeImage
    INCLUDEPATH += $$PWD/../_lib/freeimage
    DEPENDPATH += $$PWD/../_lib/freeimage

    # opencv
    CONFIG(release, debug|release) {
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_core310
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_imgproc310
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_highgui310
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_videoio310
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_calib3d310
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_optflow310
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_features2d310
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_xfeatures2d310
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_photo310
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_xphoto310
    } else {
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_core310d
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_imgproc310d
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_highgui310d
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_videoio310d
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_calib3d310d
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_optflow310d
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_features2d310d
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_xfeatures2d310d
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_photo310d
        LIBS += -L$$PWD/../_lib/opencv/x64/vc14/lib/ -lopencv_xphoto310d
    }
}



macx {
    QMAKE_MAC_SDK = macosx10.11
    LIBS += -L$$PWD/../_lib/freeimage/ -lfreeimage-3.16.0

    INCLUDEPATH += $$PWD/../_lib/freeimage
    DEPENDPATH += $$PWD/../_lib/freeimage

    #DESTDIR = ../_bin/$$CONFIGURATION/$$PLATFORM/ImagePlay.app/Contents/Frameworks/
    DESTDIR = ../_lib/

    LIBS += -L$$PWD/../_lib/opencv/x64/clang/lib/ -lopencv_core.3.1.0
    LIBS += -L$$PWD/../_lib/opencv/x64/clang/lib/ -lopencv_imgproc.3.1.0
    LIBS += -L$$PWD/../_lib/opencv/x64/clang/lib/ -lopencv_highgui.3.1.0
    LIBS += -L$$PWD/../_lib/opencv/x64/clang/lib/ -lopencv_videoio.3.1.0
    LIBS += -L$$PWD/../_lib/opencv/x64/clang/lib/ -lopencv_calib3d.3.1.0
    LIBS += -L$$PWD/../_lib/opencv/x64/clang/lib/ -lopencv_optflow.3.1.0
    LIBS += -L$$PWD/../_lib/opencv/x64/clang/lib/ -lopencv_features2d.3.1.0
    LIBS += -L$$PWD/../_lib/opencv/x64/clang/lib/ -lopencv_xfeatures2d.3.1.0
    LIBS += -L$$PWD/../_lib/opencv/x64/clang/lib/ -lopencv_photo.3.1.0
    LIBS += -L$$PWD/../_lib/opencv/x64/clang/lib/ -lopencv_xphoto.3.1.0

}

linux {
    CONFIG += staticlib

    LIBS += -lfreeimage
    LIBS += -lopencv_core
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_highgui

    DESTDIR = ../_bin/$$CONFIGURATION/$$PLATFORM/
}

msvc {
    QMAKE_CXXFLAGS += -openmp
    QMAKE_LFLAGS   += -openmp

    #QMAKE_CXXFLAGS_RELEASE -= -O1
    #QMAKE_CXXFLAGS_RELEASE -= -O2
    #QMAKE_CXXFLAGS_RELEASE *= -O3
}

clang {
    CONFIG +=c++11
    QMAKE_CXXFLAGS += -openmp
    QMAKE_LFLAGS   += -openmp
}

gcc:!clang {
    CONFIG +=c++11
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_LFLAGS   += -fopenmp
    LIBS += -lgomp
}


# IPL
INCLUDEPATH += $$PWD/include/
INCLUDEPATH += $$PWD/include/processes/

# OpenCV
INCLUDEPATH += $$PWD/include/opencv/
