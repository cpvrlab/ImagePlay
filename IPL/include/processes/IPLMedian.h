#ifndef IPLMEDIAN_H
#define IPLMEDIAN_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLMedian class
 */
class IPLSHARED_EXPORT IPLMedian : public IPLClonableProcess<IPLMedian>
{
public:
                            IPLMedian() : IPLClonableProcess() { init(); }
                            ~IPLMedian()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLMEDIAN_H
