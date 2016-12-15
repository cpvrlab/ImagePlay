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
    addOutput("Image", IPL_IMAGE_COLOR);

    // all properties which can later be changed by GUI
    addProcessPropertyString("path", "File",
                             "*.bmp, *.gif, *.hdr, *.jpg, *.png, *.psd, *.tiff, *.cr2 and many more...",
                             _path, IPL_WIDGET_FILE_OPEN);
    addProcessPropertyInt("mode", "Mode:Normal|RAW", "normal|raw", 0, IPL_WIDGET_GROUP);
    addProcessPropertyInt("raw_width", "Width", "", 512, IPL_WIDGET_SLIDER, 1, 4096);
    addProcessPropertyInt("raw_height", "Height", "", 512, IPL_WIDGET_SLIDER, 1, 4096);
    addProcessPropertyInt("raw_format", "Pixel format:8 bit (Grayscale)|24 bit (RGB)|24 bit (BGR)|32 bit (RGBA)|32 bit (ABGR)", "", 0, IPL_WIDGET_COMBOBOX);
    addProcessPropertyInt("raw_interleaved", "Byte Order:Interleaved|Planar",
                          "If you know your files's dimensions and byte order, you can load it as RAW data.",
                          0, IPL_WIDGET_COMBOBOX);
}

void IPLLoadImage::destroy()
{
    delete _result;
}

void IPLLoadImage::beforeProcessing()
{
    addInformation("beforeProcessing IPLLoadImage");
}

bool IPLLoadImage::processInputData(IPLData*, int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    // get properties
    _path           = getProcessPropertyString("path");
    int mode        = getProcessPropertyInt("mode");
    int raw_width   = getProcessPropertyInt("raw_width");
    int raw_height  = getProcessPropertyInt("raw_height");
    int raw_format  = getProcessPropertyInt("raw_format");
    int raw_interleaved  = getProcessPropertyInt("raw_interleaved");

    bool interleaved = (raw_interleaved == 0);

    if(_path.length() == 0)
    {
        addError("Image path is empty.");
        return false;
    }

    // rotating progress indicator
    notifyProgressEventHandler(-1);

    std::string information;
    bool success = false;

    // either load using the FreeImage decoder
    // or try decoding raw image data
    if(mode == 0)
        success = IPLFileIO::loadFile(_path, this->_result, information);
    else
        success = IPLFileIO::loadRawFile(_path, this->_result, raw_width, raw_height, (IPLRawImageType) raw_format, interleaved, information);

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

void IPLLoadImage::afterProcessing()
{

}

void IPLLoadImage::setPath(std::string path)
{
    IPLProcessPropertyString* pathProperty = dynamic_cast<IPLProcessPropertyString*>(this->property("path"));

    if(pathProperty)
        pathProperty->setValue(path);
}
