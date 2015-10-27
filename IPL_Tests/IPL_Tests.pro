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

QT       += testlib

QT       -= gui

TARGET = tst_ipltests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    tst_ipl.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

LIBS += -L$$PWD/../_bin/Release/x64 -lIPL

INCLUDEPATH += $$PWD/../IPL/include/
INCLUDEPATH += $$PWD/../IPL/include/processes/
INCLUDEPATH += $$PWD/../IPL/include/opencv/
DEPENDPATH += $$PWD/../IPL/include/


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
