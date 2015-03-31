//#############################################################################
//  File:      IPL_global.h
//  Author:    Kaspar Schmid
//  Date:      November 2014
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

/*!
 * \brief The IPLProcessPropertyType enum
 */
enum IPLProcessPropertyType
{
    IPL_INT,
    IPL_DOUBLE,
    IPL_INT_SLIDER,
    IPL_INT_SLIDER_ODD,
    IPL_INT_SLIDER_EVEN,
    IPL_DOUBLE_SLIDER,
    IPL_COLOR_RGB,
    IPL_COLOR_HSL,
    IPL_COLOR_HSV,
    IPL_FILE_OPEN,
    IPL_FILE_SAVE,
    IPL_FOLDER,
    IPL_BOOL_CHECKBOX,
    IPL_INT_CHECKBOXES,
    IPL_INT_RADIOBUTTONS,
    IPL_INT_COMBOBOX,
    IPL_INT_KERNEL,
    IPL_INT_BINARY_MORPHOLOGY,
    IPL_INT_BINARY_MORPHOLOGY_TRISTATE,
    IPL_INT_GRAYSCALE_MORPHOLOGY,
    IPL_DOUBLE_GAMMA,
    IPL_STRING,
    IPL_LABEL,
    IPL_POINT
};

#endif // IPL_GLOBAL_H
