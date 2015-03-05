#ifndef IPLARITHMETICOPERATIONSCONSTANT_H
#define IPLARITHMETICOPERATIONSCONSTANT_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/**
 * @brief The IPLArithmeticOperationsConstant class
 */
class IPLSHARED_EXPORT IPLArithmeticOperationsConstant : public IPLClonableProcess<IPLArithmeticOperationsConstant>
{
public:
    IPLArithmeticOperationsConstant() : IPLClonableProcess() { init(); }
    ~IPLArithmeticOperationsConstant()  { destroy(); }
    void                init();
    void                destroy();
    virtual bool        processInputData                        (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*   getResultData                           (int outNr);

protected:
    int                 _operation;
    float               _constant;
    IPLImage*           _result;
};


#endif // IPLARITHMETICOPERATIONSCONSTANT_H
