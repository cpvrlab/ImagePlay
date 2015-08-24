TEMPLATE      = lib
CONFIG       += plugin
INCLUDEPATH  += ../_lib/include/
INCLUDEPATH  += ../_lib/include/opencv/

HEADERS     += $$files(*.h,true)
SOURCES     += $$files(*.cpp,true)

TARGET = %CLASSNAME%

LIBS += -L%ROOTPATH% -lIPL
DESTDIR       = %PLUGINPATH%
