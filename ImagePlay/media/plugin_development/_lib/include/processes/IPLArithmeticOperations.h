#ifndef IPLARITHMETICOPERATIONS_H
#define IPLARITHMETICOPERATIONS_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/*!
 *
 */
class IPLSHARED_EXPORT IPLArithmeticOperations : public IPLClonableProcess<IPLArithmeticOperations>
{
public:
                        IPLArithmeticOperations     () : IPLClonableProcess() { init(); }
                        ~IPLArithmeticOperations    () { destroy(); }
    void                init                        ();
    void                destroy();
    bool                processInputData            (IPLImage* data, int inNr, bool useOpenCV);
    IPLImage*           getResultData               (int outNr);

private:
    int                 _operation;
    IPLImage*           _inputA;
    IPLImage*           _inputB;
    IPLImage*           _result;
};


#endif // IPLARITHMETICOPERATIONS_H
