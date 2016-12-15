QT += core gui network

isEmpty(FV_APP_NAME) {
	warning("Fervor: falling back to application name '$$TARGET'")
	DEFINES += FV_APP_NAME=\\\"$$TARGET\\\"
} else {
	message("Fervor: building for application name '$$FV_APP_NAME'")
	DEFINES += FV_APP_NAME=\\\"$$FV_APP_NAME\\\"
}

isEmpty(FV_APP_VERSION) {
	warning("Fervor: falling back to application version '$$VERSION'")
	DEFINES += FV_APP_VERSION=\\\"$$VERSION\\\"
} else {
	message("Fervor: building for application version '$$FV_APP_VERSION'")
	DEFINES += FV_APP_VERSION=\\\"$$FV_APP_VERSION\\\"
}

# Unit tests
#DEFINES += FV_DEBUG=1
#DEPENDPATH += "$$PWD/tests/"
#INCLUDEPATH += "$$PWD/tests/"
#CONFIG += qtestlib
#SOURCES += tests/fvversioncomparatortest.cpp
#HEADERS += tests/fvversioncomparatortest.h

DEPENDPATH += "$$PWD"
INCLUDEPATH += "$$PWD"

SOURCES += \
        $$PWD/fvupdatewindow.cpp \
        $$PWD/fvupdater.cpp \
        $$PWD/fvversioncomparator.cpp \
        $$PWD/fvplatform.cpp \
        $$PWD/fvignoredversions.cpp \
        $$PWD/fvavailableupdate.cpp \
        $$PWD/fvupdateconfirmdialog.cpp

HEADERS += \
        $$PWD/fvupdatewindow.h \
        $$PWD/fvupdater.h \
        $$PWD/fvversioncomparator.h \
        $$PWD/fvplatform.h \
        $$PWD/fvignoredversions.h \
        $$PWD/fvavailableupdate.h \
        $$PWD/fvupdateconfirmdialog.h

FORMS += $$PWD/fvupdatewindow.ui \
        $$PWD/fvupdateconfirmdialog.ui

TRANSLATIONS += $$PWD/fervor_lt.ts
CODECFORTR = UTF-8
