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
                             IPL_FILE_OPEN, _path);
}

void IPLLoadImage::destroy()
{
    delete _result;
}

bool IPLLoadImage::processInputData(IPLImage*, int, bool useOpenCV)
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
