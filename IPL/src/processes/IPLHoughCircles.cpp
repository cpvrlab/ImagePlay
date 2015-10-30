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

#include "IPLHoughCircles.h"

void IPLHoughCircles::init()
{
    // init
    _overlay        = NULL;
    _result         = NULL;

    // basic settings
    setClassName("IPLHoughCircles");
    setTitle("Circle Hough Transform");
    setCategory(IPLProcess::CATEGORY_OBJECTS);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);
    setDescription("The circle Hough Transform (CHT) is a feature extraction technique for detecting circles.");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_BW);
    addOutput("Hough Result", IPL_IMAGE_GRAYSCALE);
    addOutput("Circle Overlay", IPL_IMAGE_COLOR);
    addOutput("Circle Positions", IPL_POINT);

    // properties
    addProcessPropertyInt("thresholdCanny", "Threshold 1", "Upper threshold for the internal Canny edge detector", 200, IPL_WIDGET_SLIDER, 1, 200);
    addProcessPropertyInt("thresholdCenter", "Threshold 2", "Threshold for center detection", 100, IPL_WIDGET_SLIDER, 1, 200);
    addProcessPropertyInt("minRadius", "Min. Radius", "", 1, IPL_WIDGET_SLIDER, 1, 1000);
    addProcessPropertyInt("maxRadius", "Max. Radius", "", 5, IPL_WIDGET_SLIDER, 1, 1000);
    addProcessPropertyInt("minDist", "Min. Distance", "", 100, IPL_WIDGET_SLIDER, 1, 1000);
}

void IPLHoughCircles::destroy()
{
    //delete _result;
}
bool IPLHoughCircles::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;
    delete _overlay;
    _overlay = NULL;

    // get properties
    int thresholdCanny       = getProcessPropertyInt("thresholdCanny");
    int thresholdCenter      = getProcessPropertyInt("thresholdCenter");
    int minRadius            = getProcessPropertyInt("minRadius");
    int maxRadius            = getProcessPropertyInt("maxRadius");
    int minDist              = getProcessPropertyInt("minDist");

    notifyProgressEventHandler(-1);
    cv::Mat input;
    cv::Mat overlay = image->toCvMat();
    cv::Mat result = cv::Mat(image->height(), image->width(), CV_8UC1);
    result = cv::Scalar(0);
    cvtColor(image->toCvMat(), input, CV_BGR2GRAY);
    overlay.convertTo(overlay, CV_8UC3);

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(input, circles, CV_HOUGH_GRADIENT, 2, input.rows/4, thresholdCanny, thresholdCenter, minRadius, maxRadius);

    // WARNING: cv::HoughCircles does not work in debug mode!!!
    //          destroys the std::vector<cv::Vec3f> circles;

    std::stringstream s;
    s << "Circles found: ";
    s << circles.size();
    addInformation(s.str());

    for(int i = 0; i < circles.size(); i++ )
    {
       cv::Point center(round(circles[i][0]), round(circles[i][1]));
       int radius = cvRound(circles[i][2]);
       // circle center
       cv::circle(overlay, center, 3, cv::Scalar(0,255,0), -1, 8, 0);
       // circle outline
       cv::circle(overlay, center, radius, cv::Scalar(0,0,255), 3, 1, 0);

       // raw result
       cv::circle(result, center, radius, cv::Scalar(255), -1);
     }

    _overlay = new IPLImage(overlay);
    _result = new IPLImage(result);

	return true;
}

/*!
 * \brief IPLHoughCircles::getResultData
 *        index == 0: "Hough Result", IPL_IMAGE_GRAYSCALE
 *        index == 1: "Circle Overlay", IPL_IMAGE_COLOR
 *        index == 2: "Circle Positions", IPLImage::IMAGE_POINT
 * \return
 */
IPLData* IPLHoughCircles::getResultData(int index)
{
    if(index == 0)
        return _result;
    else if(index == 1)
        return _overlay;
    else
        return _result;
}
