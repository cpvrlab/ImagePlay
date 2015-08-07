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

    notifyProgressEventHandler(-1);

    _fileList.clear();

    // list the files
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
                _fileList.push_back(name);
        }

        closedir(d);
    }

    _sequenceCount = (int)_fileList.size();

    if(_sequenceCount < 1)
        return false;

    // play from 0 to max
    if(_sequenceIndex >= _sequenceCount)
        _sequenceIndex = 0;
    if(_sequenceIndex < 0)
        _sequenceIndex = _sequenceCount-1;

    // load current file
    std::stringstream ss;
    ss << _folder << "/" << _fileList[_sequenceIndex];
    std::string fileName = ss.str();

    std::string information;
    bool success = IPLFileIO::loadFile(fileName, this->_result, information);

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

void IPLLoadImageSequence::afterProcessing()
{
    if(_result)
    {
        _sequenceIndex++;
        notifyPropertyChangedEventHandler();
    }
}

void IPLLoadImageSequence::setFolder(std::string path)
{
    IPLProcessPropertyString* pathProperty = dynamic_cast<IPLProcessPropertyString*>(this->property("folder"));

    if(pathProperty)
        pathProperty->setValue(path);
}
