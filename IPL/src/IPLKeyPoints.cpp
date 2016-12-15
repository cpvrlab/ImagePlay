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

#include "IPLKeyPoints.h"


IPLKeyPoints::IPLKeyPoints() : IPLData(IPL_KEYPOINTS)
{
}

IPLKeyPoints::IPLKeyPoints(const IPLKeyPoints &other) : IPLData(IPL_KEYPOINTS)
{
    _keypoints = other._keypoints;
}

IPLKeyPoints::~IPLKeyPoints()
{

}

cv::KeyPoint IPLKeyPoints::get(int i)
{
    return _keypoints[i];
}

std::vector<cv::KeyPoint>* IPLKeyPoints::get()
{
    return &_keypoints;
}

void IPLKeyPoints::set(int i, cv::KeyPoint keypoint)
{
    _keypoints[i] = keypoint;
}

void IPLKeyPoints::set(std::vector<cv::KeyPoint> keypoints)
{
    _keypoints = keypoints;
}

int IPLKeyPoints::size()
{
    return _keypoints.size();
}
