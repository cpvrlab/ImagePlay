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

#include "IPLIFFT.h"


void IPLIFFT::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLIFFT");
    setTitle("Inverse FFT");
    setCategory(IPLProcess::CATEGORY_FOURIER);
    setDescription("Inverse Fast Fourier Transform.");
	setKeywords("IFFT");

    // inputs and outputs
    addInput("Complex Image", IPL_IMAGE_COMPLEX);
    addOutput("Grayscale Image", IPL_IMAGE_GRAYSCALE);

    // properties
    //addProcessPropertyInt("mode", "Windowing Function:None|Hanning|Hamming|Blackman|Border", "", IPL_INT_RADIOBUTTONS, 0);
}

void IPLIFFT::destroy()
{
    delete _result;
}

bool IPLIFFT::processInputData(IPLData* data , int, bool)
{
    IPLComplexImage* complexImageData = data->toComplexImage();
    if (NULL == complexImageData) {
        // TODO write an error message
        return false;
    }
    _complexImage = new IPLComplexImage(*complexImageData);

    // delete previous result
    delete _result;
    _result = NULL;

    int width = _complexImage->width();
    int height = _complexImage->height();

    _result = new IPLImage(IPL_IMAGE_GRAYSCALE, width, height);

    // get properties
    //int mode = getProcessPropertyInt("mode");

    int progress = 0;
    int maxProgress = height;

    _complexImage->IFFT();

    float dc = _complexImage->real(0,0);

    float min = _complexImage->minReal();
    float max = _complexImage->maxReal();

    float diff = max-min;

    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);
        for(int x=0; x<width; x++)
        {
            float value = (_complexImage->real(x,y) - min) / diff;
            value = (value < 0) ? 0 : value;
            value = (value > 1) ? 1 : value;
            _result->plane(0)->p(x, y) = value;
        }
    }

    std::stringstream s;
    s << "DC: ";
    s << dc;
    addInformation(s.str());

    return true;
}

IPLData* IPLIFFT::getResultData( int )
{
    return _result;
}
