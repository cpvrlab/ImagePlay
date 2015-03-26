#ifndef IPLHysteresisThreshold_H
#define IPLHysteresisThreshold_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLHysteresisThreshold class
 */
class IPLSHARED_EXPORT IPLHysteresisThreshold : public IPLClonableProcess<IPLHysteresisThreshold>
{
public:
                            IPLHysteresisThreshold() : IPLClonableProcess() { init(); }
                            ~IPLHysteresisThreshold()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLHysteresisThreshold_H
