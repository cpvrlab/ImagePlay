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

#include "IPLNormalizeIllumination.h"

void IPLNormalizeIllumination::init()
{
    // init
    _result         = NULL;
    _illumination   = NULL;
    _cycles = 1;
    _factor = 1;

    // basic settings
    setClassName("IPLNormalizeIllumination");
    setTitle("Normalize Illumination");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPL_IMAGE_COLOR);
    addOutput("Image", IPL_IMAGE_COLOR);
    addOutput("Illumination Image", IPL_IMAGE_COLOR);

    // set the openCV support
    setOpenCVSupport( IPLOpenCVSupport::OPENCV_ONLY );

    // properties
    addProcessPropertyInt("mean", "Local Average:Mean|Gaussian Mean|Median|Bilateral Filter", "", 0, IPL_WIDGET_RADIOBUTTONS);
    addProcessPropertyInt("cycles", "Filter Size (2n+1)", "", 10, IPL_WIDGET_SLIDER, 1, 50);
    addProcessPropertyDouble("factor", "Illumination Scale", "", 1.0, IPL_WIDGET_SLIDER, 0.0, 5.0);
}

void IPLNormalizeIllumination::destroy()
{
    delete _result;
    delete _illumination;
}

bool IPLNormalizeIllumination::processInputData(IPLData* data, int, bool)
{
    IPLImage* image = data->toImage();
	
    // delete previous result
    delete _result;
    _result = NULL;
    delete _illumination;
    _illumination = NULL;

    // get properties
    _factor = getProcessPropertyDouble("factor");
    _cycles = 1 + 2*getProcessPropertyInt("cycles");
    int meanSel = getProcessPropertyInt("mean");

    cv::Mat bgr32;
    cv::Mat labMat;
    cv::Mat splitLabMats[3];
    cv::Mat *lMat;
    image->toCvMat().convertTo(bgr32,CV_32F);
    bgr32 *= 1.f/255.f;
    cvtColor(bgr32,labMat,cv::COLOR_BGR2Lab);
    split(labMat,splitLabMats);
    // grab the lightness channel
    lMat = &splitLabMats[0];

    cv::Mat mu;
    cv::Mat result;
    cv::Size sqCycles(_cycles,_cycles);

    switch (meanSel){
       case 0:
         blur(*lMat,mu,sqCycles);
         break;
       case 1:
         GaussianBlur(*lMat, mu, sqCycles, 0);
         break;
       case 2:
         if (_cycles > 5 ){
           cv::Mat medIn,medMu;
           lMat->convertTo(medIn,CV_8U);
           medianBlur(medIn,medMu,_cycles);
           medMu.convertTo(mu,CV_32F);
         }else{
           medianBlur(*lMat,mu,_cycles);
         }
         break;
       case 3:
        if(_cycles > 15)
           addWarning("Large filter sizes can lead to long processing time.");
         bilateralFilter(*lMat,mu,_cycles,30.0,30.0);
         break;
    }
    mu *= _factor;
    cv::Scalar lightMean = cv::mean( *lMat );

    // subtract the localize mean and add the global mean
    // to the lightness channel, a and b stay the same
    result = *lMat - mu + lightMean;

    cv::Mat rgbResult32;
    cv::Mat rgbResult;

    // replace the lightness channel in the original with the modified
    int from_to[] = { 0,0 };
    mixChannels(&result,1,&labMat,1,from_to,1);

    cvtColor(labMat,rgbResult32,cv::COLOR_Lab2BGR);
    rgbResult32 *= 255.f;
    rgbResult32.convertTo(rgbResult,CV_8U);

    _result = new IPLImage(rgbResult);
    _illumination = new IPLImage( mu );

    notifyProgressEventHandler(100);
    return true;
}

IPLData* IPLNormalizeIllumination::getResultData(int index)
{
    if(index == 1)
        return _illumination;

    return _result;
}
