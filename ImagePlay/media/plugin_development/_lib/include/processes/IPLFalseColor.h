#ifndef IPLFalseColor_H
#define IPLFalseColor_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLFalseColor class
 */
class IPLSHARED_EXPORT IPLFalseColor : public IPLClonableProcess<IPLFalseColor>
{
public:
                            IPLFalseColor() : IPLClonableProcess() { init(); }
                            ~IPLFalseColor()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLFalseColor_H
