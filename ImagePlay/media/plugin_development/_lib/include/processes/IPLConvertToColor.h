#ifndef IPLCONVERTTOCOLOR_H
#define IPLCONVERTTOCOLOR_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/*!
 * Converts a 1-channel grayscale or b/w image to 3-channel RGB
 */
class IPLSHARED_EXPORT IPLConvertToColor : public IPLClonableProcess<IPLConvertToColor>
{
public:
    IPLConvertToColor() : IPLClonableProcess() { init(); }
    ~IPLConvertToColor()  { destroy(); }

    virtual void            init                        ();
    virtual void            destroy();
    virtual bool            processInputData            (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData               (int outNr);

protected:
    int                     _operation;
    IPLImage*               _inputA;
    IPLImage*               _inputB;
    IPLImage*               _result;
};


#endif // IPLCONVERTTOCOLOR_H
