#ifndef IPLEXTRACTLINES_H
#define IPLEXTRACTLINES_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>

/**
 * @brief The IPLExtractLines class
 */
class IPLSHARED_EXPORT IPLExtractLines : public IPLClonableProcess<IPLExtractLines>
{
public:
                            IPLExtractLines() : IPLClonableProcess() { init(); }
                            ~IPLExtractLines()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLEXTRACTLINES_H
