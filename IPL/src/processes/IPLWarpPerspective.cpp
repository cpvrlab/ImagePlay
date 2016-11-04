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

#include "IPLWarpPerspective.h"

void IPLWarpPerspective::init()
{
    // init
    _result     = NULL;
    _inputA     = NULL;
    _inputB     = NULL;
    _previewA   = NULL;
    _previewB   = NULL;

    // basic settings
    setClassName("IPLWarpPerspective");
    setTitle("Warp Perspective");
    setDescription("Applies a perspective transformation to an image.");
    setCategory(IPLProcess::CATEGORY_GEOMETRY);
    setOpenCVSupport(IPLProcess::OPENCV_ONLY);
    setKeywords("warp, transformation");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addInput("Image 2", IPL_IMAGE_COLOR);
    addOutput("Result", IPL_IMAGE_COLOR);
    addOutput("Preview A", IPL_IMAGE_COLOR);
    addOutput("Preview B", IPL_IMAGE_COLOR);

    // properties
    std::vector<double> value = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    addProcessPropertyInt("method", "Mode:Matrix|Points", "matrix|points", 0 ,IPL_WIDGET_GROUP);
    addProcessPropertyVectorDouble("matrix_M", "M:3|3", "Transformation Matrix", value, IPL_WIDGET_MATRIX);
    addProcessPropertyString("label0", "label", "Points in Image A", "", IPL_WIDGET_LABEL);
    addProcessPropertyPoint("points_a0", "Point 1", "", IPLPoint(0,0));
    addProcessPropertyPoint("points_a1", "Point 2", "", IPLPoint(0,0));
    addProcessPropertyPoint("points_a2", "Point 3", "", IPLPoint(0,0));
    addProcessPropertyPoint("points_a3", "Point 4", "", IPLPoint(0,0));
    addProcessPropertyString("label1", "label", "Points in Image B", "", IPL_WIDGET_LABEL);
    addProcessPropertyPoint("points_b0", "Point 1", "", IPLPoint(0,0));
    addProcessPropertyPoint("points_b1", "Point 2", "", IPLPoint(0,0));
    addProcessPropertyPoint("points_b2", "Point 3", "", IPLPoint(0,0));
    addProcessPropertyPoint("points_b3", "Point 4", "", IPLPoint(0,0));
    addProcessPropertyInt("interpolation", "Interpolation:Nearest|Linear|Area|Cubic|Lanczos4", "", 0, IPL_WIDGET_COMBOBOX, 0, 4);
    addProcessPropertyInt("border", "Border:Black|Replicate|Reflect|Wrap", "", 0, IPL_WIDGET_COMBOBOX, 0, 3);
}

void IPLWarpPerspective::destroy()
{
    delete _result;
}

bool IPLWarpPerspective::processInputData(IPLData* data, int imageIndex, bool)
{
    IPLImage* image = data->toImage();

    // save the first image
    if(imageIndex == 0)
    {
        delete _inputA;
        _inputA = new IPLImage(*image);
    }

    // save the second image
    if(imageIndex == 1)
    {
        delete _inputB;
        _inputB = new IPLImage(*image);
    }

    // only continue if we have 2 valid inputs
    if(!(_inputA && _inputB))
    {
        return false;
    }


    // get properties
    int method = getProcessPropertyInt("method");
    std::vector<double> v = getProcessPropertyVectorDouble("matrix_M");
    IPLPoint pointA0 = getProcessPropertyPoint("points_a0");
    IPLPoint pointA1 = getProcessPropertyPoint("points_a1");
    IPLPoint pointA2 = getProcessPropertyPoint("points_a2");
    IPLPoint pointA3 = getProcessPropertyPoint("points_a3");
    IPLPoint pointB0 = getProcessPropertyPoint("points_b0");
    IPLPoint pointB1 = getProcessPropertyPoint("points_b1");
    IPLPoint pointB2 = getProcessPropertyPoint("points_b2");
    IPLPoint pointB3 = getProcessPropertyPoint("points_b3");
    int interpolation = getProcessPropertyInt("interpolation");
    int border = getProcessPropertyInt("border");

    notifyProgressEventHandler(-1);

    // convert vector to cv::Mat
    cv::Mat matrix(3, 3, CV_32FC1);
    for(int i=0; i < (int) v.size(); i++)
    {
        int col = i % matrix.cols;
        int row = i / matrix.cols;
        matrix.at<float>(row, col) = v[i];
    }

    // automatically calculate the matrix
    cv::Point2f pointsA[4];
    cv::Point2f pointsB[4];
    pointsA[0] = cv::Point2f(pointA0.x(), pointA0.y());
    pointsA[1] = cv::Point2f(pointA1.x(), pointA1.y());
    pointsA[2] = cv::Point2f(pointA2.x(), pointA2.y());
    pointsA[3] = cv::Point2f(pointA3.x(), pointA3.y());

    pointsB[0] = cv::Point2f(pointB0.x(), pointB0.y());
    pointsB[1] = cv::Point2f(pointB1.x(), pointB1.y());
    pointsB[2] = cv::Point2f(pointB2.x(), pointB2.y());
    pointsB[3] = cv::Point2f(pointB3.x(), pointB3.y());

    // only in point mode
    if(method == 1)
    {
        matrix = cv::getPerspectiveTransform(pointsB, pointsA);
    }

    cv::Mat result;
    cv::warpPerspective(_inputB->toCvMat(), result, matrix, cv::Size(image->width(), image->height()), interpolation, border);

    cv::Mat previewA = _inputA->toCvMat();
    cv::circle(previewA, pointsA[0], 3, cv::Scalar(0,0,255), -1, 8, 0);
    cv::circle(previewA, pointsA[1], 3, cv::Scalar(0,0,255), -1, 8, 0);
    cv::circle(previewA, pointsA[2], 3, cv::Scalar(0,0,255), -1, 8, 0);
    cv::circle(previewA, pointsA[3], 3, cv::Scalar(0,0,255), -1, 8, 0);

    cv::Mat previewB = _inputB->toCvMat();
    cv::circle(previewB, pointsB[0], 3, cv::Scalar(0,255,0), -1, 8, 0);
    cv::circle(previewB, pointsB[1], 3, cv::Scalar(0,255,0), -1, 8, 0);
    cv::circle(previewB, pointsB[2], 3, cv::Scalar(0,255,0), -1, 8, 0);
    cv::circle(previewB, pointsB[3], 3, cv::Scalar(0,255,0), -1, 8, 0);

    delete _result;
    _result = new IPLImage(result);
    delete _previewA;
    _previewA = new IPLImage(previewA);
    delete _previewB;
    _previewB = new IPLImage(previewB);

    return true;
}

IPLData* IPLWarpPerspective::getResultData(int index)
{
    switch (index) {
    case 1:
        return _previewA;
        break;
    case 2:
        return _previewB;
        break;
    default:
        return _result;
    }
}
