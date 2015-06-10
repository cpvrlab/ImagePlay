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

#include "IPL_global.h"

const char* const widgetNames[IPL_NUM_WIDGETS] = {
    "IPL_WIDGET_DEFAULT",
    "IPL_WIDGET_HIDDEN",
    "IPL_WIDGET_CHECKBOXES",
    "IPL_WIDGET_RADIOBUTTONS",
    "IPL_WIDGET_COMBOBOX",
    "IPL_WIDGET_SLIDER",
    "IPL_WIDGET_SPINNER",
    "IPL_WIDGET_SLIDER_ODD",
    "IPL_WIDGET_SLIDER_EVEN",
    "IPL_WIDGET_TEXTFIELD",
    "IPL_WIDGET_LABEL",
    "IPL_WIDGET_TITLE",
    "IPL_WIDGET_FILE_OPEN",
    "IPL_WIDGET_FILE_SAVE",
    "IPL_WIDGET_FOLDER",
    "IPL_WIDGET_KERNEL",
    "IPL_WIDGET_BINARY_MORPHOLOGY",
    "IPL_WIDGET_BINARY_MORPHOLOGY_TRISTATE",
    "IPL_WIDGET_GRAYSCALE_MORPHOLOGY",
    "IPL_WIDGET_COLOR_RGB",
    "IPL_WIDGET_COLOR_HSL",
    "IPL_WIDGET_COLOR_HSV",
    "IPL_WIDGET_POINT",
    "IPL_WIDGET_GROUP"
};

const char *widgetName(IPLProcessWidgetType type)
{
    if (type < 0 || type >= IPL_NUM_WIDGETS)
        return "UNKNOWN";
    return widgetNames[type];
}
