#ifndef IPLMERGEPLANES_H
#define IPLMERGEPLANES_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>
#include <vector>

/**
 * @brief The IPLSplitPlanes class
 */
class IPLSHARED_EXPORT IPLMergePlanes : public IPLClonableProcess<IPLMergePlanes>
{
public:
    IPLMergePlanes() : IPLClonableProcess() { init(); }
    ~IPLMergePlanes()  { destroy(); }

    void init();
    virtual void        destroy();

    virtual bool processInputData( IPLImage* data, int inNr, bool useOpenCV );
    virtual IPLImage* getResultData( int outputIndex );

protected:
    IPLImage*   _inputA;
    IPLImage*   _inputB;
    IPLImage*   _inputC;
    IPLImage*   _result;
    int         _inputType;
};

#endif // IPLMERGEPLANES_H
