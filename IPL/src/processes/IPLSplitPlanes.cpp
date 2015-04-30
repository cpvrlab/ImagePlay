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

#include "IPLSplitPlanes.h"

void IPLSplitPlanes::init()
{
    // init
    _outputType = 0;
    _hueShift   = 0;

    // basic settings
    setClassName("IPLSplitPlanes");
    setTitle("Split Planes");
    setKeywords("RGB, HSV, HSL, channels");
    setCategory(IPLProcess::CATEGORY_CONVERSIONS);

    // properties
    addProcessPropertyInt("output_type", "Color Model:RGB|HSV|HSL", "", _outputType, IPL_WIDGET_RADIOBUTTONS);
    addProcessPropertyInt("hue_shift", "Hue Shift", "0° - 360°", _hueShift, IPL_WIDGET_SLIDER, 0, 360);

    // inputs and outputs
    addInput("Color Image", IPLData::IMAGE_COLOR);

    addOutput("Channel 1", IPLData::IMAGE_GRAYSCALE);
    addOutput("Channel 2", IPLData::IMAGE_GRAYSCALE);
    addOutput("Channel 3", IPLData::IMAGE_GRAYSCALE);

}

void IPLSplitPlanes::destroy()
{
    for(uint i=0; i<_result.size(); i++)
    {
        delete _result[i];
    }
}

bool IPLSplitPlanes::processInputData(IPLImage* image, int, bool)
{
    // delete previous result
    for(uint i=0; i<_result.size(); i++)
    {
        delete _result[i];
    }
    _result.clear();

    int width = image->width();
    int height = image->height();

    // get properties
    int outputType = getProcessPropertyInt("output_type");
    float hueShift = getProcessPropertyInt("hue_shift") / 360.0;

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();

    // RGB
    if(outputType == 0)
    {
        setOutputName(0, "Red");
        setOutputName(1, "Green");
        setOutputName(2, "Blue");

        for( int planeNr=0; planeNr < image->getNumberOfPlanes(); planeNr++ )
        {
            IPLImage* newImage = new IPLImage(IPLData::IMAGE_GRAYSCALE, width, height);
            _result.push_back(newImage);

            // copy data
            for(int y=0; y<height; y++)
            {
                // progress
                notifyProgressEventHandler(100*progress++/maxProgress);

                for(int x=0; x<width; x++)
                {
                    newImage->plane(0)->p(x, y) = image->plane(planeNr)->p(x, y);
                }
            }
        }
    }
    // HSV
    else if(outputType == 1)
    {
        IPLImage* image_h = new IPLImage(IPLData::IMAGE_GRAYSCALE, width, height);
        IPLImage* image_s = new IPLImage(IPLData::IMAGE_GRAYSCALE, width, height);
        IPLImage* image_v = new IPLImage(IPLData::IMAGE_GRAYSCALE, width, height);
        _result.push_back(image_h);
        _result.push_back(image_s);
        _result.push_back(image_v);

        IPLImagePlane* input_r = image->plane(0);
        IPLImagePlane* input_g = image->plane(1);
        IPLImagePlane* input_b = image->plane(2);

        setOutputName(0, "Hue");
        setOutputName(1, "Saturation");
        setOutputName(2, "Value");

        // copy data
        for(int y=0; y<height; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int x=0; x<width; x++)
            {
                float hsv[3];
                IPLColor::rgbToHsv(input_r->p(x,y), input_g->p(x,y), input_b->p(x,y), hsv);
                float hue = fmod(hsv[0] + hueShift, 1.0);
                image_h->plane(0)->p(x, y) = hue;
                image_s->plane(0)->p(x, y) = hsv[1];
                image_v->plane(0)->p(x, y) = hsv[2];
            }
        }
    }
    // HSL
    else if(outputType == 2)
    {
        IPLImage* image_h = new IPLImage(IPLData::IMAGE_GRAYSCALE, width, height);
        IPLImage* image_s = new IPLImage(IPLData::IMAGE_GRAYSCALE, width, height);
        IPLImage* image_l = new IPLImage(IPLData::IMAGE_GRAYSCALE, width, height);
        _result.push_back(image_h);
        _result.push_back(image_s);
        _result.push_back(image_l);

        IPLImagePlane* input_r = image->plane(0);
        IPLImagePlane* input_g = image->plane(1);
        IPLImagePlane* input_b = image->plane(2);

        setOutputName(0, "Hue");
        setOutputName(1, "Saturation");
        setOutputName(2, "Lightness");

        // copy data
        for(int y=0; y<height; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int x=0; x<width; x++)
            {
                float hsl[3];
                IPLColor::rgbToHsl(input_r->p(x,y), input_g->p(x,y), input_b->p(x,y), hsl);
                float hue = fmod(hsl[0] + hueShift, 1.0);
                image_h->plane(0)->p(x, y) = hue;
                image_s->plane(0)->p(x, y) = hsl[1];
                image_l->plane(0)->p(x, y) = hsl[2];
            }
        }
    }

    return true;
}

IPLImage* IPLSplitPlanes::getResultData( int outputIndex )
{
    if(!isResultReady())
        return NULL;

    if(outputIndex < _result.size())
        return _result[outputIndex];
    else
        return NULL;

}

