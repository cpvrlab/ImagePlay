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

#ifndef IPL_GLOBAL_H
#define IPL_GLOBAL_H

#define NOMINMAX

//#include <QtCore/qglobal.h>

#include <algorithm>
#include <math.h>

#if defined(__linux__) || defined(__APPLE__)
    #define EXTERNC         extern "C"
    #define IPLSHARED_EXPORT     __attribute__((visibility("default")))
    #define IPLSHARED_IMPORT     __attribute__((visibility("default")))
    #define IPLSHARED_HIDDEN     __attribute__((visibility("hidden")))
#endif

#if _WIN32
    #if defined(IPL_LIBRARY)
    #  define IPLSHARED_EXPORT __declspec(dllexport)
    #else
    #  define IPLSHARED_EXPORT __declspec(dllimport)
    #endif

    #pragma warning(disable:4251) // class 'type' needs to have dll-interface to be used by clients of class 'type2'
    #pragma warning(disable:4100) // unreferenced formal parameter
    #pragma warning(disable:4996) // strtok unsafe
    #pragma warning(disable:4244) // possible loss of data
    #define _CRT_SECURE_NO_WARNINGS // this function or variable may be unsave
#endif

#if defined(IPL_LIBRARY)
#  ifndef HIBYTE
#    define HIBYTE(x)   ((unsigned int)(x) >> 8)
#    define LOWBYTE(x)  ((unsigned char)(x))
#  endif
#endif

typedef unsigned char   uchar;
typedef unsigned int    uint;
typedef float           ipl_basetype;

#define FACTOR_TO_FLOAT (1.0f / 255.0f)
#define FACTOR_TO_UCHAR 255.0f

#define PI 3.14159265359

enum IPLProcessWidgetType
{
    IPL_WIDGET_DEFAULT = 0,
    IPL_WIDGET_HIDDEN,

    //Bitfield or Booleans
    IPL_WIDGET_CHECKBOXES,

    //Enumerated integer
    IPL_WIDGET_RADIOBUTTONS,
    IPL_WIDGET_COMBOBOX,

    //Numeric
    IPL_WIDGET_SLIDER,
    IPL_WIDGET_SPINNER,

    //Integer
    IPL_WIDGET_SLIDER_ODD,
    IPL_WIDGET_SLIDER_EVEN,

    //String
    IPL_WIDGET_TEXTFIELD,
    IPL_WIDGET_LABEL,
    IPL_WIDGET_TITLE,
    IPL_WIDGET_FILE_OPEN,
    IPL_WIDGET_FILE_SAVE,
    IPL_WIDGET_FOLDER,

    //Kernel
    IPL_WIDGET_KERNEL,

    //Morphology
    IPL_WIDGET_BINARY_MORPHOLOGY,
    IPL_WIDGET_BINARY_MORPHOLOGY_TRISTATE,
    IPL_WIDGET_GRAYSCALE_MORPHOLOGY,

    //Color
    IPL_WIDGET_COLOR_RGB,
    IPL_WIDGET_COLOR_HSL,
    IPL_WIDGET_COLOR_HSV,

    //Points
    IPL_WIDGET_POINT,

    //Integer: A button that increments the property after each click
    IPL_WIDGET_BUTTON,

    IPL_WIDGET_GROUP,

    IPL_WIDGET_MATRIX,

    //Add additional widget types here
    //...

    IPL_NUM_WIDGETS
};

const char *widgetName(IPLProcessWidgetType type);

#endif // IPL_GLOBAL_H
