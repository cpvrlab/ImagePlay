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

#include "IPLGradientOperator.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

void IPLGradientOperator::init()
{
    // init
    _result = NULL;

    // basic settings
    setClassName("IPLGradientOperator");
    setTitle("Gradient Operator");
    setCategory(IPLProcess::CATEGORY_GRADIENTS);
    setKeywords("Fast Gradient, Roberts, Sobel, Cubic Spline");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_GRAYSCALE);
    addOutput("Image", IPLData::IMAGE_ORIENTED);

    // properties
    addProcessPropertyInt("algorithm", "Algorithm:Fast Gradient|Roberts|Sobel|Cubic Spline", "", 0, IPL_WIDGET_RADIOBUTTONS);
}

void IPLGradientOperator::destroy()
{
    delete _result;
}

bool IPLGradientOperator::processInputData(IPLImage* image , int, bool useOpenCV)
{
    // delete previous result
    delete _result;
    _result = new IPLOrientedImage(image->width(), image->height());

    // get properties
    int algorithm = getProcessPropertyInt("algorithm");
    std::cout << "Gradient OP = " << algorithm << "\n";

    switch (algorithm){
       case 0:
       default:
          return fastGradient(image);
       case 1:
          return roberts(image, useOpenCV);
       case 2:
          return sobel(image, useOpenCV);
       case 3:
          return cubicSpline(image, useOpenCV);
    }

    //make compiler happy...
    return false;
}

bool IPLGradientOperator::fastGradient(IPLImage* image)
{
    int width = image->width();
    int height = image->height();

    // fast gradient
    int progress = 0;
    int maxProgress = height*width;
    for(int x=1; x<width; x++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);
        for(int y=1; y<height; y++)
        {
            double Dx = image->plane(0)->p(x,y) - image->plane(0)->p(x-1, y);
            double Dy = image->plane(0)->p(x,y) - image->plane(0)->p(x, y-1);
            double magnitude = sqrt( Dx * Dx + Dy * Dy );
            double phase = (Dx!=0.0 || Dy!=0.0 )? atan2( -Dy, Dx ) : 0.0;

            while( phase > 2.0 * PI ) phase -= 2.0 * PI;
            while( phase < 0.0 ) phase += 2.0 * PI;

            // phase 0.0-1.0
            phase /= 2 * PI;

            _result->phase(x,y) = phase;
            _result->magnitude(x,y) = magnitude;
        }
    }

    return true;
}

bool IPLGradientOperator::roberts(IPLImage* image, bool useOpenCV)
{
    static float rxf[2][2] = {{1.0,0},{0,-1.0}};
    static cv::Mat rxKernel(2,2,CV_32FC1,rxf);
    static float ryf[2][2] = {{0,1.0},{-1.0,0}};
    static cv::Mat ryKernel(2,2,CV_32FC1,ryf);

    int width = image->width();
    int height = image->height();

    // fast gradient
    int progress = 0;
    int maxProgress = height*width;

    notifyProgressEventHandler(-1);

  if ( useOpenCV ){
    cv::Mat input;
    cv::Mat gX;
    cv::Mat gY;
    cvtColor(image->toCvMat(),input,CV_BGR2GRAY);

    filter2D(input,gX,CV_32F,rxKernel);
    filter2D(input,gY,CV_32F,ryKernel);

    for(int x=1; x<width; x++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);
        for(int y=1; y<height; y++)
        {
          ipl_basetype gx = gX.at<cv::Vec<float,1>>(y,x).val[0] * FACTOR_TO_FLOAT ;
          ipl_basetype gy = gY.at<cv::Vec<float,1>>(y,x).val[0] * FACTOR_TO_FLOAT ;

          double phase = (gx!=0.0 || gy!=0.0 )? atan2( -gy, gx ) : 0.0;

          while( phase > 2.0 * PI ) phase -= 2.0 * PI;
          while( phase < 0.0 ) phase += 2.0 * PI;

          // phase 0.0-1.0
          phase /= 2 * PI;

          _result->phase(x,y) = phase;
          _result->magnitude(x,y) = sqrt(gx*gx + gy*gy);
        }
    }

   }else{

   }

   return true;
}

bool IPLGradientOperator::sobel(IPLImage* image, bool useOpenCV)
{
    int width = image->width();
    int height = image->height();

    const int kSize = 3;

    // fast gradient
    int progress = 0;
    int maxProgress = height*width;

    notifyProgressEventHandler(-1);

  if ( useOpenCV ){
    cv::Mat input;
    cv::Mat gX;
    cv::Mat gY;
    cvtColor(image->toCvMat(),input,CV_BGR2GRAY);

    Sobel(input,gX,CV_32F,1,0,kSize);
    Sobel(input,gY,CV_32F,0,1,kSize);

    std::cout << gX.channels() << " channels\n";

    for(int x=1; x<width; x++)
    {
        for(int y=1; y<height; y++)
        {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);
          ipl_basetype gx = gX.at<cv::Vec<float,1>>(y,x).val[0] * FACTOR_TO_FLOAT ;
          ipl_basetype gy = gY.at<cv::Vec<float,1>>(y,x).val[0] * FACTOR_TO_FLOAT ;
          
          double phase = (gx!=0.0 || gy!=0.0 )? atan2( -gy, gx ) : 0.0;

          while( phase > 2.0 * PI ) phase -= 2.0 * PI;
          while( phase < 0.0 ) phase += 2.0 * PI;

          phase /= 2.0 * PI;

          _result->phase(x,y) = phase;
          _result->magnitude(x,y) = sqrt(gx*gx + gy*gy);
        }
     }
  }else{

  }
  return true;
}

bool IPLGradientOperator::cubicSpline(IPLImage* image, bool useOpenCV)
{
  static float k1f[4][4] = {{-0.5,1.5,-1.5,0.5},{1.0,-2.5,2.0,-0.5},{-0.5,0,0.5,0},{0,1.0,0,0}};
  static cv::Mat k1(4,4,CV_32FC1,k1f);
  static float k2f[4][4] = {{-0.5,1.0,-0.5,0},{1.5,-2.5,0,1},{-1.5,2.0,0.5,0},{0.5,-0.5,0,0}};
  static cv::Mat k2(4,4,CV_32FC1,k2f);

  int width = image->width();
  int height = image->height();
  int progress = 0;
  int maxProgress = height*width;
 
  notifyProgressEventHandler(-1);

  if ( useOpenCV ){
    cv::Mat input = image->toCvMat();
    cv::Mat coeff = cv::Mat::zeros(4,4,CV_32FC1);

    float dx,dy,x3,x2,y3,y2,xf,yf;
    
    for (int x=1; x < width - 2; x++){
      for (int y = 1; y < height - 2; y++){
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);

         for (int i=0;i<4;i++)
           for (int j=0;j<4;j++)
             coeff.at<cv::Vec<float,1>>(i,j)[0] = static_cast<float>(input.at<cv::Vec<unsigned char,4>>(y-1+i,x-1+j).val[0]) * FACTOR_TO_FLOAT ;

         coeff = k1*coeff;
         coeff = coeff*k2;
         xf = static_cast<float>(x)/static_cast<float>(width);
         x2 = xf*xf;
         x3 = x2*xf;
         yf = static_cast<float>(y)/static_cast<float>(height);
         y2 = yf*yf;
         y3 = y2*yf;
	 float dyMatf[4][4] = {{3.f*y2*x3,3.f*y2*x2,3.f*y2*xf,3.f*y2},
				{2.f*yf*x3,2.f*yf*x2,2.f*yf*xf,2.f*yf},
				{x3,x2,xf,1.f},
				{0.f,0.f,0.f,0.f}};
	 float dxMatf[4][4] = {{3.f*x2*y3,2.f*xf*y3,y3,0.f},
				{3.f*x2*y2,2.f*xf*y2,y2,0.f},
				{3.f*x2*yf,2.f*xf*yf,yf,0.f},
				{3.f*x2,2.f*xf,1.f,0.f}};
         cv::Mat dxMat(4,4,CV_32FC1,dxMatf);
         cv::Mat dyMat(4,4,CV_32FC1,dyMatf);
         dx = coeff.dot(dxMat);
         dy = coeff.dot(dyMat);

         double phase = (dx!=0.0 || dy!=0.0 )? atan2( -dy, dx ) : 0.0;

          while( phase > 2.0 * PI ) phase -= 2.0 * PI;
          while( phase < 0.0 ) phase += 2.0 * PI;

          // phase 0.0-1.0
          phase /= 2 * PI;

          _result->phase(x,y) = phase;
          _result->magnitude(x,y) = sqrt(dx*dx + dy*dy);
     }
    }
  }else{

  }
  return true;
}

IPLImage* IPLGradientOperator::getResultData( int )
{
    return _result;
}
