#ifndef IPLEqualizeHistogram_H
#define IPLEqualizeHistogram_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLEqualizeHistogram class
 */
class IPLSHARED_EXPORT IPLEqualizeHistogram : public IPLClonableProcess<IPLEqualizeHistogram>
{
public:
                            IPLEqualizeHistogram() : IPLClonableProcess() { init(); }
                            ~IPLEqualizeHistogram()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLEqualizeHistogram_H
