#ifndef IPLLocalThreshold_H
#define IPLLocalThreshold_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLLocalThreshold class
 */
class IPLSHARED_EXPORT IPLLocalThreshold : public IPLClonableProcess<IPLLocalThreshold>
{
public:
                            IPLLocalThreshold() : IPLClonableProcess() { init(); }
                            ~IPLLocalThreshold()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLLocalThreshold_H
