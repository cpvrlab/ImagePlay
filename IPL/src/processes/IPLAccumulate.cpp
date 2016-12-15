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

#include "IPLAccumulate.h"

void IPLAccumulate::init()
{
    // init
    _result         = NULL;

    // basic settings
    setClassName("IPLAccumulate");
    setTitle("Accumulate");
    setCategory(IPLProcess::CATEGORY_OBJECTS);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);
    setDescription("Adds an image to the accumulator.");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);

    // properties
    addProcessPropertyInt("method", "Method:Default|Square|Product|Weighted", "default|square|product|weighted", 0, IPL_WIDGET_GROUP);
    addProcessPropertyDouble("weighted_weight", "Weight", "", 0.5, IPL_WIDGET_SLIDER, 0.0, 2.0);
}

void IPLAccumulate::destroy()
{
    //delete _result;
}

bool IPLAccumulate::processInputData(IPLData* data , int, bool)
{
    IPLImage* image = data->toImage();

    // init result
    if(!_result)
    {
        int width = image->width();
        int height = image->height();

        _result = new IPLImage(image->type(), width, height);
        _result->fillColor(0.0);
    }

    // get properties
    int method              = getProcessPropertyInt("method");
    double weight           = getProcessPropertyDouble("weighted_weight");

    /*std::stringstream s;
    s << "Window: ";
    s << window;
    addInformation(s.str());*/

    notifyProgressEventHandler(-1);
    cv::Mat input = image->toCvMat();
    cv::Mat output = _result->toCvMat();

    input.convertTo(input, CV_32FC3);
    output.convertTo(output, CV_32FC3);


    if(method == 0)
    {
        cv::accumulate(input, output);
        output *= 0.5;
    }
    else if(method == 1)
    {
        cv::accumulateSquare(input, output);
    }
    else if(method == 2)
    {
        cv::accumulateProduct(input, input, output);
    }
    else if(method == 3)
    {
        cv::accumulateWeighted(input, output, weight);
    }

    output.convertTo(output, CV_8UC3);

    delete _result;
    _result = new IPLImage(output);

    return true;
}

IPLData* IPLAccumulate::getResultData( int )
{
    return _result;
}
