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

#include "IPLRotate.h"

void IPLRotate::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLRotate");
    setTitle("Rotate/Zoom Image");
    setCategory(IPLProcess::CATEGORY_GEOMETRY);
    setOpenCVSupport(IPLProcess::OPENCV_ONLY);

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);

    // properties
    addProcessPropertyInt("angle", "Angle", "", 0, IPL_WIDGET_SLIDER, -180, 180);
    addProcessPropertyDouble("scale", "Scaling", "", 1.0, IPL_WIDGET_SLIDER, 0.1, 10.0);
    addProcessPropertyPoint("center", "Center", "", IPLPoint(0,0), IPL_WIDGET_POINT);
    addProcessPropertyInt("interpolation", "Interpolation:Nearest|Linear|Area|Cubic|Lanczos4", "", 0, IPL_WIDGET_COMBOBOX, 0, 4);
}

void IPLRotate::destroy()
{
    delete _result;
}

bool IPLRotate::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    // get properties
    int angle = getProcessPropertyInt("angle");
    double scale = getProcessPropertyDouble("scale");
    int interpolation = getProcessPropertyInt("interpolation");
    IPLPoint center = getProcessPropertyPoint("center");

    // Compute a rotation matrix with respect to the center of the image
    //cv::Point centerPoint = cv::Point( image->width()/2, image->height()/2 );
    cv::Point centerPoint = cv::Point(center.x(), center.y());

    /*std::stringstream s;
    s << "<b>Original Aspect Ratio</b>: " << aspectRatio << "\n";
    s << "<b>New Aspect Ratio</b>: " << newAspectRatio << "\n";
    s << "<b>Height</b>: " << height << "\n";
    s << "<b>Width</b>: " << width;
    addInformation(s.str());*/

    _result = new IPLImage(image->type(), image->width(), image->height());

    cv::Mat result;

    // Get the rotation matrix with the specifications above
    cv::Mat rot_mat = getRotationMatrix2D(centerPoint, angle, scale);

    notifyProgressEventHandler(-1);

    // Rotate the warped image
    cv::warpAffine(image->toCvMat(), result, rot_mat, result.size(), interpolation);

    delete _result;
    _result = new IPLImage(result);

    return true;
}

IPLData* IPLRotate::getResultData(int)
{
    return _result;
}
