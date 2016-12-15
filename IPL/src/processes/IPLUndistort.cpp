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

#include "IPLUndistort.h"

void IPLUndistort::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLUndistort");
    setTitle("Undistort Image");
    setDescription("The function transforms an image to compensate radial and tangential lens distortion.");
    setCategory(IPLProcess::CATEGORY_GEOMETRY);
    setOpenCVSupport(IPLProcess::OPENCV_ONLY);
    setKeywords("distortion, undistortion, barrel, lens correction");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);

    // properties
    addProcessPropertyInt("f", "f", "Focal Length", 1000, IPL_WIDGET_SLIDER, 0, 10000.0);

    addProcessPropertyDouble("p1", "p1", "", 0.0, IPL_WIDGET_SLIDER, -10.0, 10.0);
    addProcessPropertyDouble("p2", "p2", "Tangential Distortion", 0.0, IPL_WIDGET_SLIDER, -10.0, 10.0);
    addProcessPropertyDouble("k1", "k1", "", 0.0, IPL_WIDGET_SLIDER, -100.0, 100.0);
    addProcessPropertyDouble("k2", "k2", "", 0.0, IPL_WIDGET_SLIDER, -100.0, 100.0);
    addProcessPropertyDouble("k3", "k3", "Radial Distortion", 0.0, IPL_WIDGET_SLIDER, -100.0, 100.0);
}

void IPLUndistort::destroy()
{
    delete _result;
}

bool IPLUndistort::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // get properties
    float k1 = getProcessPropertyDouble("k1");
    float k2 = getProcessPropertyDouble("k2");
    float k3 = getProcessPropertyDouble("k3");
    float p1 = getProcessPropertyDouble("p1");
    float p2 = getProcessPropertyDouble("p2");

    float c1 = getProcessPropertyInt("f");
    float c2 = c1;

    notifyProgressEventHandler(-1);

    cv::Mat cameraMatrix = (cv::Mat_<double>(3,3) << c1, 0, image->width()*0.5, 0, c2, image->height()*0.5, 0, 0, 1);


    cv::Mat distCoeffs(5, 1, CV_32FC1);
    distCoeffs.at<float>(0,0) = k1;
    distCoeffs.at<float>(1,0) = k2;
    distCoeffs.at<float>(2,0) = p1;
    distCoeffs.at<float>(3,0) = p2;
    distCoeffs.at<float>(4,0) = k3;

    cv::Mat result;
    cv::undistort(image->toCvMat(), result, cameraMatrix, distCoeffs);

    delete _result;
    _result = new IPLImage(result);

    return true;
}

IPLData* IPLUndistort::getResultData(int)
{
    return _result;
}
