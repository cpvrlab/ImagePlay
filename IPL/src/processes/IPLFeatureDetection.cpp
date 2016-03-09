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

#include "IPLFeatureDetection.h"

void IPLFeatureDetection::init()
{
    // init
    _image    = NULL;
    _preview   = NULL;
    _keypoints = NULL;

    // basic settings
    setClassName("IPLFeatureDetection");
    setTitle("Feature Detection");
    setCategory(IPLProcess::CATEGORY_OBJECTS);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Preview", IPL_IMAGE_COLOR);
    addOutput("KeyPoints", IPL_KEYPOINTS);

    // properties
    addProcessPropertyInt("algorithm", "Algorithm:FAST|MSER|ORB|BRISK|FREAK", "", 0, IPL_WIDGET_COMBOBOX);
    addProcessPropertyDouble("threshold", "Threshold", "", 0.0, IPL_WIDGET_SLIDER, 0.0, 255.0);
    addProcessPropertyBool("nonmaxSuppression", "Non Maxima Suppression", "", false);
    addProcessPropertyInt("minHessian", "minHessian", "", 1, IPL_WIDGET_SLIDER, 1, 1000);
}

void IPLFeatureDetection::destroy()
{
    delete _image;
    delete _preview;

}

bool IPLFeatureDetection::processInputData(IPLData* data, int, bool useOpenCV)
{
    IPLImage* image = data->toImage();
	
    int width = image->width();
    int height = image->height();
    int progress = 0;
    int maxProgress = ((image->type() == IPL_IMAGE_COLOR)? 9 : 8)*height;

    _keypoints = new IPLKeyPoints();

    // get properties
    int algorithm              = getProcessPropertyInt("algorithm");
    double threshold           = getProcessPropertyDouble("threshold");
    bool nonmaxSuppression     = getProcessPropertyBool("nonmaxSuppression");
    int minHessian             = getProcessPropertyInt("minHessian");

    notifyProgressEventHandler(-1);
    cv::Mat input;
    cv::Mat output;
    cvtColor(image->toCvMat(), input, CV_BGR2GRAY);
    input.copyTo(output);

    //cv::OrbFeatureDetector detector;
    std::vector<cv::KeyPoint> keypoints;
    //cv::Ptr<cv::ORB> detector = cv::ORB::create(300, 1.2f, 4, 31, 0, 2, cv::ORB::FAST_SCORE, 31, 20);
    cv::FAST(input, keypoints, threshold, nonmaxSuppression);
    //cv::Ptr<cv::SURF> detector = cv::SURF::create( minHessian );
    //detector.detect(input, keypoints);
    cv::drawKeypoints(input, keypoints, output);

    delete _preview;
    _preview = new IPLImage(output);
    _keypoints = new IPLKeyPoints;
    _keypoints->set(keypoints);

    return true;
}

IPLData* IPLFeatureDetection::getResultData( int index )
{
    if(index == 1)
        return _keypoints;
    else
        return _preview;
}
