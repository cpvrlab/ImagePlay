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

#ifndef IPLLOADIMAGESEQUENCE_H
#define IPLLOADIMAGESEQUENCE_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLFileIO.h"

#include <string>
#include <stdio.h>
#if defined(__linux__) || defined(__APPLE__)
    #include <sys/uio.h>
    #include <dirent.h>
#else
    #include <io.h>
    #include "dirent/dirent.h"
#endif
#include <iostream>

/**
 * @brief The IPLLoadImageSequence class
 */
class IPLSHARED_EXPORT IPLLoadImageSequence : public IPLClonableProcess<IPLLoadImageSequence>
{
public:
    IPLLoadImageSequence() : IPLClonableProcess() { init(); }
    ~IPLLoadImageSequence()  { destroy(); }

    void init();
    void destroy();
    virtual bool processInputData( IPLImage* data, int inNr, bool useOpenCV );
    virtual IPLImage* getResultData( int outNr );
    int sequenceCount()                         { return _sequenceCount; }
    int sequenceIndex()                         { return _sequenceIndex; }
    void setSequenceIndex(int sequenceIndex)    { _sequenceIndex = sequenceIndex; }
    void afterProcessing();
protected:
    IPLImage*   _result;
    std::string _folder;
    int _sequenceCount;
    int _sequenceIndex;
};

#endif // IPLLOADIMAGESEQUENCE_H
