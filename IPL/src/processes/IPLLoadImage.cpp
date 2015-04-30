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

#include "IPLLoadImage.h"

void IPLLoadImage::init()
{
    // init
    _result = NULL;
    _path = "";

    // basic settings
    setClassName("IPLLoadImage");
    setTitle("Load Image");
    setCategory(IPLProcess::CATEGORY_IO);
    setIsSource(true);

    setHelpPage("helppage.html");

    // inputs and outputs
    addOutput("Image", IPLData::IMAGE_COLOR);

    // all properties which can later be changed by GUI
    addProcessPropertyString("path", "File",
                             "*.bmp, *.gif, *.hdr, *.jpg, *.png, *.psd, *.tiff, *.cr2 and many more...",
                             _path, IPL_WIDGET_FILE_OPEN);
}

void IPLLoadImage::destroy()
{
    delete _result;
}

bool IPLLoadImage::processInputData(IPLImage*, int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    // get properties
    _path = getProcessPropertyString("path");

    if(_path.length() == 0)
    {
        addError("Image path is empty.");
        return false;
    }

    notifyProgressEventHandler(50);
    std::string information;
    bool success = IPLFileIO::loadFile(_path, this->_result, information);
    if(success)
    {
        addInformation(information);
    }
    else
    {
        addError("Could not load image file: " + _path);
    }
    return success;
}

IPLImage *IPLLoadImage::getResultData(int)
{
    return _result;
}
