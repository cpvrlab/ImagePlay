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

#include "IPLGoodFeaturesToTrack.h"

void IPLGoodFeaturesToTrack::init()
{
    // init
    _overlay = NULL;
    _result  = NULL;
    _corners = NULL;

    // basic settings
    setClassName("IPLGoodFeaturesToTrack");
    setTitle("Good Features To Track");
    setCategory(IPLProcess::CATEGORY_OBJECTS);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);
    setDescription("Shi-Tomasi Corner Detector & Good Features to Track.");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_GRAYSCALE); // Input 8-bit or floating-point 32-bit, single-channel image.
    addOutput("Corners overlay", IPL_IMAGE_COLOR); // index 0 (overlay)
    addOutput("Corners (raw)", IPL_IMAGE_GRAYSCALE); // index 1 (result)
    // Not implemented (later, when <cv::Vec2f> to IPLKeypoints is implemented
    addOutput("Corners positions", IPL_KEYPOINTS); // index 2 (corners)

    // properties
    addProcessPropertyInt("maxCorners", "Max corners", "Maximum number of corners to return. If there are more corners than are found, the strongest of them is returned.", 200, IPL_WIDGET_SLIDER, 1, 200);
    addProcessPropertyDouble("qualityLevel", "Quality level", "Parameter characterizing the minimal accepted quality of image corners.", 0.01, IPL_WIDGET_SLIDER, 0.001, 15);
    addProcessPropertyDouble("minDistance", "Min. distance", "Minimum possible Euclidean distance between the returned corners.", 20, IPL_WIDGET_SLIDER, 1, 1000);
    addProcessPropertyInt("block_size", "Block size", "Size of an average block for computing a derivative covariation matrix over each pixel neighborhood.", 3, IPL_WIDGET_SLIDER, 1, 250);
    addProcessPropertyBool("useHarrisDetector", "Harris detector", "Whether to use a Harris detector", false, IPL_WIDGET_CHECKBOXES);
    addProcessPropertyDouble("k", "k", "Free parameter of the Harris detector.", 0.04, IPL_WIDGET_SLIDER, 0, 1);
}

void IPLGoodFeaturesToTrack::destroy()
{
    //delete _result;
}
bool IPLGoodFeaturesToTrack::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;
    delete _overlay;
    _overlay = NULL;

    // get properties
    int maxCorners         = getProcessPropertyInt("maxCorners");
    double qualityLevel    = getProcessPropertyDouble("qualityLevel");
    double minDistance     = getProcessPropertyDouble("minDistance");
    int block_size         = getProcessPropertyInt("block_size");
    bool useHarrisDetector = getProcessPropertyBool("useHarrisDetector");
    double k               = getProcessPropertyDouble("k");

    notifyProgressEventHandler(-1);
    cv::Mat input;
    cv::Mat mask; // ToDo (as optional input?)
    cv::Mat overlay = image->toCvMat();
    cv::Mat result = cv::Mat(image->height(), image->width(), CV_8UC1);
    result = cv::Scalar(0);
    cvtColor(image->toCvMat(), input, CV_BGR2GRAY);

    std::vector<cv::Vec2f> corners;
    cv::goodFeaturesToTrack(input, corners, maxCorners, qualityLevel, minDistance, cv::noArray(), block_size, useHarrisDetector, k);

    std::stringstream s;
    s << "Corners found: ";
    s << corners.size();
    addInformation(s.str());

    for(int i = 0; i < (int) corners.size(); i++)
    {
       cv::Point center(round(corners[i][0]), round(corners[i][1]));
       cv::circle(overlay, center, 5, cv::Scalar(0,255,0), -1, 8, 0);
       cv::circle(result, center, 5, cv::Scalar(255), -1);
     }

    _overlay = new IPLImage(overlay);
    _result = new IPLImage(result);
    // _corners = corners;

    return true;
}

IPLData* IPLGoodFeaturesToTrack::getResultData(int index)
{
    if(index == 0)
        return _overlay;
    else // if(index == 1)
        return _result;
    /*else if(index == 1)
        return _corners;*/
}
