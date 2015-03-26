#ifndef IPLMAX_H
#define IPLMAX_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLMax class
 */
class IPLSHARED_EXPORT IPLMax : public IPLClonableProcess<IPLMax>
{
public:
                            IPLMax() : IPLClonableProcess() { init(); }
                            ~IPLMax()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLMAX_H
