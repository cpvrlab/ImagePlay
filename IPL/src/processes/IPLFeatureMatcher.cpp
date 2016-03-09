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

#include "IPLFeatureMatcher.h"

void IPLFeatureMatcher::init()
{
    // init
    _image    = NULL;
    _preview   = NULL;

    // basic settings
    setClassName("IPLFeatureMatcher");
    setTitle("Feature Matcher");
    setCategory(IPLProcess::CATEGORY_OBJECTS);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addInput("KeyPoints 1", IPL_KEYPOINTS);
    addInput("KeyPoints 2", IPL_KEYPOINTS);
    addOutput("Preview", IPL_IMAGE_COLOR);

    // properties
    addProcessPropertyInt("algorithm", "Algorithm:FAST|MSER|ORB|BRISK|FREAK", "", 0, IPL_WIDGET_COMBOBOX);
    addProcessPropertyDouble("threshold", "Threshold", "", 0.0, IPL_WIDGET_SLIDER, 0.0, 255.0);
    addProcessPropertyBool("nonmaxSuppression", "Non Maxima Suppression", "", false);
    addProcessPropertyInt("minHessian", "minHessian", "", 1, IPL_WIDGET_SLIDER, 1, 1000);
}

void IPLFeatureMatcher::destroy()
{
    delete _image;
    delete _preview;

}

bool IPLFeatureMatcher::processInputData(IPLData* data , int index, bool useOpenCV)
{
    IPLImage* image = data->toImage();
	
    if(index == 0)
    {
        _image = data->toImage();
    }
    else if(index == 1)
    {
        _keypoints1 = data->toKeyPoints();
    }
    else
    {
        return false;
    }

    // get properties
    int algorithm              = getProcessPropertyInt("algorithm");
    double threshold           = getProcessPropertyDouble("threshold");
    bool nonmaxSuppression     = getProcessPropertyBool("nonmaxSuppression");
    int minHessian             = getProcessPropertyInt("minHessian");

    notifyProgressEventHandler(-1);
    cv::Mat input;
    cv::Mat output;
    std::vector<cv::KeyPoint> keypoints = *_keypoints1->get();
    cvtColor(_image->toCvMat(), input, CV_BGR2GRAY);

    cv::drawKeypoints(input, keypoints, output);

    delete _preview;
    _preview = new IPLImage(output);

    return true;
}

IPLData* IPLFeatureMatcher::getResultData( int index )
{
    return _preview;
}
