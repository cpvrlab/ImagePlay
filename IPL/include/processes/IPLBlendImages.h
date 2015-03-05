#ifndef IPLCOMBINEIMAGES_H
#define IPLCOMBINEIMAGES_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>
#include <cmath>

/**
 * @brief The IPLBlendImages class
 */
class IPLSHARED_EXPORT IPLBlendImages : public IPLClonableProcess<IPLBlendImages>
{
public:
    IPLBlendImages() : IPLClonableProcess() { init(); }
    ~IPLBlendImages()  { destroy(); }

    void                    init();
    virtual void            destroy();

    virtual bool            processInputData    (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData       (int outNr);

protected:
    IPLImage*               _result;
    IPLImage*               _inputA;
    IPLImage*               _inputB;
    int                     _operation;
    double                  _factorA;
    double                  _factorB;
};

#endif // IPLCOMBINEIMAGES_H
