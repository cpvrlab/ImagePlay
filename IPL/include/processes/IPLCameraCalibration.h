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

#ifndef IPLCAMERACALIBRATION_H
#define IPLCAMERACALIBRATION_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLKeyPoints.h"

#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>

enum { DETECTION = 0, CALIBRATION = 1, CALIBRATED = 2 };
enum Pattern { CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };

/**
 * @brief The IPLCameraCalibration class
 */
class IPLSHARED_EXPORT IPLCameraCalibration : public IPLClonableProcess<IPLCameraCalibration>
{
public:
                            IPLCameraCalibration() : IPLClonableProcess() { init(); }
                            ~IPLCameraCalibration()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    void                    processPropertyEvents   (IPLEvent*);
    bool                    processInputData        (IPLData*, int, bool useOpenCV);
    IPLData*                getResultData           (int);
protected:
    int                     _mode;
    int                     _frameCounter;
    IPLImage*               _image;
    IPLImage*               _preview;
    std::vector<std::vector<cv::Point2f>> _imagePoints;

private:
    bool runCalibration(std::vector<std::vector<cv::Point2f> > imagePoints, cv::Size imageSize, cv::Size boardSize, Pattern patternType, float squareSize, float aspectRatio, int flags, cv::Mat &cameraMatrix, cv::Mat &distCoeffs, std::vector<cv::Mat> &rvecs, std::vector<cv::Mat> &tvecs, std::vector<float> &reprojErrs, double &totalAvgErr);
    void calcChessboardCorners(cv::Size boardSize, float squareSize, std::vector<cv::Point3f> &corners, Pattern patternType);
    double computeReprojectionErrors(const std::vector<std::vector<cv::Point3f> > &objectPoints, const std::vector<std::vector<cv::Point2f> > &imagePoints, const std::vector<cv::Mat> &rvecs, const std::vector<cv::Mat> &tvecs, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs, std::vector<float> &perViewErrors);
};

#endif // IPLCameraCalibration_H
