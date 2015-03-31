#include "IPLLoadImageSequence.h"

void IPLLoadImageSequence::init()
{
    // init
    _result         = NULL;
    _folder         = "";
    _sequenceCount  = 0;
    _sequenceIndex  = 0;

    // basic settings
    setClassName("IPLLoadImageSequence");
    setTitle("Load Sequence");
    setCategory(IPLProcess::CATEGORY_IO);
    setIsSource(true);

    // inputs and outputs
    addOutput("Image", IPLData::IMAGE_COLOR);

    // all properties which can later be changed by gui
    addProcessPropertyString("folder", "Folder", "", _folder, IPL_WIDGET_FOLDER);
}

void IPLLoadImageSequence::destroy()
{
    delete _result;
}

bool IPLLoadImageSequence::processInputData(IPLImage*, int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    // get properties
    _folder = getProcessPropertyString("folder");

    notifyProgressEventHandler(50);

    // list the files
    std::vector<std::string> fileList;

    DIR *d;
    struct dirent *dir;
    int i=0;
    d = opendir(_folder.c_str());
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            i++;
            std::string name(dir->d_name);
            if(name != "." && name != "..")
                fileList.push_back(name);
        }

        closedir(d);
    }

    _sequenceCount = fileList.size();

    if(_sequenceCount < 1)
    {
        return false;
    }

    // play from 0 to max
    if(_sequenceIndex >= _sequenceCount)
    {
        _sequenceIndex = 0;
    }

    // load current file
    std::stringstream ss;
    ss << _folder << "/" << fileList[_sequenceIndex];
    std::string fileName = ss.str();

    std::string information;
    bool success = IPLFileIO::loadFile(fileName, this->_result, information);

    _sequenceIndex++;

    std::stringstream s;
    s << "File: ";
    s << (_sequenceIndex+1);
    s << " / ";
    s << _sequenceCount;
    addInformation(s.str());

    return success;
}

IPLImage *IPLLoadImageSequence::getResultData(int)
{
    return _result;
}
