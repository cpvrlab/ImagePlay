#ifndef IPLEquidensityLines_H
#define IPLEquidensityLines_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLEquidensityLines class
 */
class IPLSHARED_EXPORT IPLEquidensityLines : public IPLClonableProcess<IPLEquidensityLines>
{
public:
                            IPLEquidensityLines() : IPLClonableProcess() { init(); }
                            ~IPLEquidensityLines()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLEquidensityLines_H
