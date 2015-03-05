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
protected:
    IPLImage*   _result;
    std::string _folder;
    int _sequenceCount;
    int _sequenceIndex;
};

#endif // IPLLOADIMAGESEQUENCE_H
