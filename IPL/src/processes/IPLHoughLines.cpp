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

#include "IPLHoughLines.h"

void IPLHoughLines::init()
{
    // init
    _overlay        = NULL;
    _result         = NULL;

    // basic settings
    setClassName("IPLHoughLines");
    setTitle("Hough Lines");
    setCategory(IPLProcess::CATEGORY_OBJECTS);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);
    setDescription("Finds lines in a binary image using the standard Hough transform.");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Hough Result", IPLImage::IMAGE_GRAYSCALE);
    addOutput("Line Overlay", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyDouble("rho", "Rho", "Distance resolution of the accumulator in pixels", 1, IPL_WIDGET_SLIDER, 0, 10);
    addProcessPropertyDouble("theta", "Min. Radius", "Angle resolution of the accumulator in radians.", 0.01, IPL_WIDGET_SLIDER, 0, 5.14);
    addProcessPropertyInt("threshold", "Threshold", "Accumulator threshold parameter.", 0, IPL_WIDGET_SLIDER, 1, 1000);
    addProcessPropertyInt("minLenght", "Min. Length", "", 1, IPL_WIDGET_SLIDER, 1, 1000);
    addProcessPropertyInt("maxLineGap", "Max. Line Gap", "", 1, IPL_WIDGET_SLIDER, 1, 1000);
}

void IPLHoughLines::destroy()
{
    //delete _result;
}
bool IPLHoughLines::processInputData(IPLImage* image , int, bool useOpenCV)
{
    // delete previous result
    delete _result;
    _result = NULL;
    delete _overlay;
    _overlay = NULL;

    // get properties
    double rho              = getProcessPropertyDouble("rho");
    double theta            = getProcessPropertyDouble("theta");
    int threshold           = getProcessPropertyInt("threshold");
    int minLength           = getProcessPropertyInt("minLenght");
    int maxLineGap          = getProcessPropertyInt("maxLineGap");

    notifyProgressEventHandler(-1);
    cv::Mat input;
    cv::Mat overlay = image->toCvMat();
    cv::Mat result = cv::Mat(image->height(), image->width(), CV_8UC1);
    result = cv::Scalar(0);
    cvtColor(image->toCvMat(), input, CV_BGR2GRAY);

    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(input, lines, rho, theta, threshold, minLength, maxLineGap);

    std::stringstream s;
    s << "Lines found: ";
    s << lines.size();
    addInformation(s.str());

    for(int i = 0; i < lines.size(); i++ )
    {
       cv::Vec4i l = lines[i];
       cv::line(overlay, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3, CV_AA);

       // raw result
       cv::line(result, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255), 1, CV_AA);
     }

    _overlay = new IPLImage(overlay);
    _result = new IPLImage(result);

	return true;
}

/*!
 * \brief IPLHoughLines::getResultData
 *        index == 0: "Hough Result", IPLImage::IMAGE_GRAYSCALE
 *        index == 1: "Circle Overlay", IPLImage::IMAGE_COLOR
 * \return
 */
IPLData* IPLHoughLines::getResultData(int index)
{
    if(index == 0)
        return _result;
    else
        return _overlay;
}
