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

    // basic settings
    setClassName("IPLWarpPerspective");
    setTitle("Warp Perspective");
    setDescription("Applies a perspective transformation to an image.");
    setCategory(IPLProcess::CATEGORY_GEOMETRY);
    setOpenCVSupport(IPLProcess::OPENCV_ONLY);
    setKeywords("warp, transformation");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);

    // properties
    std::vector<double> value = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    addProcessPropertyVectorDouble("M", "M:3|3", "Transformation Matrix", value, IPL_WIDGET_MATRIX);
    addProcessPropertyInt("interpolation", "Interpolation:Nearest|Linear|Area|Cubic|Lanczos4", "", 0, IPL_WIDGET_COMBOBOX, 0, 4);
    addProcessPropertyInt("border", "Border:Black|Replicate|Reflect|Wrap", "", 0, IPL_WIDGET_COMBOBOX, 0, 3);
}

void IPLWarpPerspective::destroy()
{
    delete _result;
}

bool IPLWarpPerspective::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // get properties
    std::vector<double> v = getProcessPropertyVectorDouble("M");
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

    cv::Mat result;
    cv::warpPerspective(image->toCvMat(), result, matrix, cv::Size(image->width(), image->height()), interpolation, border);

    delete _result;
    _result = new IPLImage(result);

    return true;
}

IPLData* IPLWarpPerspective::getResultData(int)
{
    return _result;
}
