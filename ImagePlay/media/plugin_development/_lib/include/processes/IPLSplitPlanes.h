#ifndef IPLSPLITPLANES_H
#define IPLSPLITPLANES_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>
#include <vector>

/**
 * @brief The IPLSplitPlanes class
 */
class IPLSHARED_EXPORT IPLSplitPlanes : public IPLClonableProcess<IPLSplitPlanes>
{
public:
    IPLSplitPlanes() : IPLClonableProcess() { init(); }
    ~IPLSplitPlanes()  { destroy(); }

    void init();
    virtual void        destroy();

    virtual bool processInputData( IPLImage* data, int inNr, bool useOpenCV );
    virtual IPLImage* getResultData( int outputIndex );

protected:
    std::vector<IPLImage*>   _result;
    int _outputType;
    int _hueShift;
};


#endif // IPLSPLITPLANES_H
