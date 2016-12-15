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

using namespace cv::xfeatures2d;

void IPLFeatureMatcher::init()
{
    // init
    _image1     = NULL;
    _image2     = NULL;
    _preview    = NULL;
    _keypoints1 = NULL;
    _keypoints2 = NULL;

    // basic settings
    setClassName("IPLFeatureMatcher");
    setTitle("Feature Matcher");
    setCategory(IPLProcess::CATEGORY_OBJECTS);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);

    // inputs and outputs
    addInput("Image 1", IPL_IMAGE_COLOR);
    addInput("Image 2", IPL_IMAGE_COLOR);
    addInput("KeyPoints 1", IPL_KEYPOINTS);
    addInput("KeyPoints 2", IPL_KEYPOINTS);
    addOutput("Preview", IPL_IMAGE_COLOR);

    // properties
    addProcessPropertyInt("algorithm", "Algorithm:FAST|MSER|ORB|BRISK|FREAK", "", 0, IPL_WIDGET_COMBOBOX);
    addProcessPropertyInt("hessianThreshold", "Hessian Threshold", "", 100, IPL_WIDGET_SLIDER, 1, 1000);
    addProcessPropertyInt("nOctaves", "n Octaves", "", 4, IPL_WIDGET_SLIDER, 1, 10);
    addProcessPropertyInt("nOctaveLayers", "n Octave Layers", "", 3, IPL_WIDGET_SLIDER, 1, 10);
    addProcessPropertyBool("extended", "Extended", "", false);
    addProcessPropertyBool("upright", "Upright", "", false);
}

void IPLFeatureMatcher::destroy()
{
    delete _image1;
    delete _image2;
    delete _keypoints1;
    delete _keypoints2;
    delete _preview;

}

bool IPLFeatureMatcher::processInputData(IPLData* data , int index, bool useOpenCV)
{
    if(index == 0)
    {
        _image1 = data->toImage();
    }
    else if(index == 1)
    {
        _image2 = data->toImage();
    }
    else if(index == 2)
    {
        std::stringstream s;
        s << "Number of Keypoints 1: ";
        s << data->toKeyPoints()->size();
        addInformation(s.str());
        _keypoints1 = data->toKeyPoints();
    }
    else if(index == 3)
    {
        _keypoints2 = data->toKeyPoints();
    }
    else
    {
        return false;
    }

    if(_image1 && _image2 && _keypoints1 && _keypoints2) {

        // get properties
        int algorithm           = getProcessPropertyInt("algorithm");
        int hessianThreshold    = getProcessPropertyInt("hessianThreshold");
        int nOctaves            = getProcessPropertyInt("nOctaves");
        int nOctaveLayers       = getProcessPropertyInt("nOctaveLayers");
        bool extended           = getProcessPropertyBool("extended");
        bool upright            = getProcessPropertyBool("upright");

        notifyProgressEventHandler(-1);
        cv::Mat input1;
        cv::Mat input2;
        cv::Mat output;
        std::vector<cv::KeyPoint> keypoints1 = *_keypoints1->get();
        std::vector<cv::KeyPoint> keypoints2 = *_keypoints2->get();
        cv::cvtColor(_image1->toCvMat(), input1, CV_BGR2GRAY);
        cv::cvtColor(_image2->toCvMat(), input2, CV_BGR2GRAY);

        std::stringstream s1;
        s1 << "Number of Keypoints 1: ";
        s1 << keypoints1.size();
        addInformation(s1.str());
        std::stringstream s2;
        s2 << "Number of Keypoints 2: ";
        s2 << keypoints2.size();
        addInformation(s2.str());

        // computing descriptors
        cv::Ptr<SURF> extractor = SURF::create(hessianThreshold, nOctaves, nOctaveLayers, extended, upright);
        cv::Mat descriptors1, descriptors2;
        extractor->compute(input1, keypoints1, descriptors1);
        extractor->compute(input2, keypoints2, descriptors2);

        std::vector<cv::DMatch> matches;
        cv::BFMatcher matcher;
        matcher.match(descriptors1, descriptors2, matches);

        std::vector<cv::DMatch> good_matches;
        double max_dist = 0; double min_dist = DBL_MAX;
        for( int i = 0; i < descriptors1.rows; i++ )
        { double dist = matches[i].distance;
          if( dist < min_dist ) min_dist = dist;
          if( dist > max_dist ) max_dist = dist;
        }
        for( int i = 0; i < descriptors1.rows; i++ )
        { if( matches[i].distance <= std::max(2*min_dist, 0.02) )
          { good_matches.push_back( matches[i]); }
        }

        std::stringstream s3;
        s3 << "Good Matches: ";
        s3 << matches.size();
        addInformation(s3.str());

        cv::drawMatches(input1, keypoints1, input2, keypoints2, good_matches, output, cv::Scalar::all(-1), cv::Scalar::all(-1),
                        std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

        delete _preview;
        _preview = new IPLImage(output);

        return true;
    }
}

IPLData* IPLFeatureMatcher::getResultData( int index )
{
    return _preview;
}
