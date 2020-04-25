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

#include "IPLOpticalFlow.h"

void IPLOpticalFlow::init()
{
    // init
    _image    = NULL;
    _previewX  = NULL;
    _previewY  = NULL;
    _keypoints = NULL;

    // basic settings
    setClassName("IPLOpticalFlow");
    setTitle("Optical Flow");
    setCategory(IPLProcess::CATEGORY_OBJECTS);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Preview", IPL_IMAGE_COLOR);
    addOutput("KeyPoints", IPL_KEYPOINTS);

    // properties
    addProcessPropertyInt("algorithm", "Algorithm:FAST|MSER|ORB|BRISK|FREAK", "", 0, IPL_WIDGET_COMBOBOX);
    addProcessPropertyDouble("threshold", "Threshold", "", 0.0, IPL_WIDGET_SLIDER, 0.0, 1.0);
    addProcessPropertyBool("nonmaxSuppression", "Non Maxima Suppression", "", false);
}

void IPLOpticalFlow::destroy()
{
    delete _image;
    delete _previewX;
    delete _previewY;

}

bool IPLOpticalFlow::processInputData(IPLData* data, int, bool useOpenCV)
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

    notifyProgressEventHandler(-1);

    // Obtain first image
    if(_image_prev.cols == 0)
        _image_prev = image->toCvMat();
        //cvtColor(image->toCvMat(), _image_prev, cv::COLOR_BGR2GRAY);

    //cvtColor(image->toCvMat(), _image_next, cv::COLOR_BGR2GRAY);
    _image_next = image->toCvMat();

    cv::Mat flow;

    cv::optflow::calcOpticalFlowSF(_image_prev, _image_next, flow, 3, 2, 4);

    flow *= 10;

    cv::Mat planes[2];
    cv::split(flow, planes);

    _image_next.copyTo(_image_prev);

    delete _previewX;
    _previewX = new IPLImage(planes[0]);
    delete _previewY;
    _previewY = new IPLImage(planes[1]);

    return true;
}

IPLData* IPLOpticalFlow::getResultData( int index )
{
    if(index == 0)
        return _previewX;
    else
        return _previewY;
}
