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

#include "IPLSobel.h"

void IPLSobel::init()
{
    // init
    _result         = NULL;

    // basic settings
    setClassName("IPLSobel");
    setTitle("Laplacian");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);
    setDescription("");

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Magnitude", IPL_IMAGE_GRAYSCALE);
    addOutput("Edge", IPL_IMAGE_GRAYSCALE);
    addOutput("Gradient", IPL_IMAGE_GRAYSCALE);

    // properties
    addProcessPropertyInt("threshold", "Threshold", "", 1, IPL_WIDGET_SLIDER, 1, 255);
    addProcessPropertyDouble("highThreshold", "High Threshold", "Thresholds for the hysteresis procedure", 0.6, IPL_WIDGET_SLIDER, 0.0, 1.0);
}

void IPLSobel::destroy()
{
    //delete _result;
}

bool IPLSobel::processInputData(IPLData* data, int, bool useOpenCV)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();

    _result = new IPLImage( image->type(), width, height );

    // get properties
    int threshold           = getProcessPropertyInt("threshold");
    /*double sigma            = getProcessPropertyDouble("sigma");
    double lowThreshold     = getProcessPropertyDouble("lowThreshold");
    double highThreshold    = getProcessPropertyDouble("highThreshold");*/

    /*std::stringstream s;
    s << "Window: ";
    s << window;
    addInformation(s.str());*/

    notifyProgressEventHandler(-1);
    cv::Mat input;
    cv::Mat output;
    cvtColor(image->toCvMat(), input, CV_BGR2GRAY);

    /// Detector parameters
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;

    cv::Mat dst_norm;

    /// Detecting corners
    cv::cornerHarris(input, output, blockSize, apertureSize, k, cv::BORDER_DEFAULT );

    /// Normalizing
    cv::normalize( output, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );
    cv::convertScaleAbs( dst_norm, output );

    cvtColor(output, output, CV_GRAY2BGR);

    /// Drawing a circle around corners
    for( int j = 0; j < dst_norm.rows ; j++ )
    {
        for( int i = 0; i < dst_norm.cols; i++ )
        {
            if( (int) dst_norm.at<float>(j,i) > threshold )
            {
                cv::line(output, cv::Point(i-3, j), cv::Point(i+3, j), cv::Scalar(0,0,255));
                cv::line(output, cv::Point(i, j-3), cv::Point(i, j+3), cv::Scalar(0,0,255));
            }
        }
    }

    delete _result;
    _result = new IPLImage(output);

    return true;
}

IPLData* IPLSobel::getResultData( int )
{
    return _result;
}
