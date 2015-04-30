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

#include "IPLResize.h"

void IPLResize::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLResize");
    setTitle("Resize Image");
    setCategory(IPLProcess::CATEGORY_GEOMETRY);
    setOpenCVSupport(IPLProcess::OPENCV_ONLY);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLData::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("mode", "Mode:Absolute|Factor", "Uses either height and width or factor x and factor y", 0, IPL_WIDGET_COMBOBOX, 0, 1);
    addProcessPropertyInt("width", "Width", "", 512, IPL_WIDGET_SLIDER, 1, 4096);
    addProcessPropertyInt("height", "Height", "", 512, IPL_WIDGET_SLIDER, 1, 4096);
    addProcessPropertyDouble("factor_x", "Factor X", "", 1.0, IPL_WIDGET_SLIDER, 0.1, 10.0);
    addProcessPropertyDouble("factor_y", "Factor Y", "", 1.0, IPL_WIDGET_SLIDER, 0.1, 10.0);
    addProcessPropertyInt("keep_aspect_ratio", "Keep Aspect Ratio:None|Based on width|Based on height", "", 0, IPL_WIDGET_COMBOBOX, 0, 1);
    addProcessPropertyInt("interpolation", "Interpolation:Nearest|Linear|Area|Cubic|Lanczos4", "", 0, IPL_WIDGET_COMBOBOX, 0, 4);
}

void IPLResize::destroy()
{
    delete _result;
}

bool IPLResize::processInputData(IPLImage* image , int, bool)
{
    // get properties
    int mode = getProcessPropertyInt("mode");
    int width = getProcessPropertyInt("width");
    int height = getProcessPropertyInt("height");
    double factor_x = getProcessPropertyDouble("factor_x");
    double factor_y = getProcessPropertyDouble("factor_y");
    int interpolation = getProcessPropertyInt("interpolation");
    int keep_aspect_ratio = getProcessPropertyInt("keep_aspect_ratio");

    // calculate height/width based on aspect_ratio
    double aspectRatio = 1.0;
    if(image->height() > 0)
        aspectRatio = (double) image->width()/image->height();
    if(keep_aspect_ratio == 1)
    {
        // calculate height
        height = width / aspectRatio;
        factor_y = factor_x;
    }
    else if(keep_aspect_ratio == 2)
    {
        // calculate width
        width = height * aspectRatio;
        factor_x = factor_y;
    }

    double newAspectRatio = (double) width / height;
    if(mode == 1)
        newAspectRatio = (double) (width*factor_x) / (height*factor_y);

    std::stringstream s;
    s << "<b>Original Aspect Ratio</b>: " << aspectRatio << "\n";
    s << "<b>New Aspect Ratio</b>: " << newAspectRatio << "\n";
    s << "<b>Height</b>: " << height << "\n";
    s << "<b>Width</b>: " << width;
    addInformation(s.str());

    cv::Mat result;
    if(mode == 0)
        cv::resize(image->toCvMat(), result, cv::Size(width, height), 0, 0, interpolation);
    else
        cv::resize(image->toCvMat(), result, cv::Size(0, 0), factor_x, factor_y, interpolation);

    delete _result;
    _result = new IPLImage(result);

    return true;
}

IPLData* IPLResize::getResultData(int)
{
    return _result;
}
