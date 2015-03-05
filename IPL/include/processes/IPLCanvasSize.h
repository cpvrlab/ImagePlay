#ifndef IPLCANVASSIZE_H
#define IPLCANVASSIZE_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>

/**
 * @brief The IPLCanvasSize class
 */
class IPLSHARED_EXPORT IPLCanvasSize : public IPLClonableProcess<IPLCanvasSize>
{
public:
    IPLCanvasSize() : IPLClonableProcess() { init(); }
    ~IPLCanvasSize()  { destroy(); }

    void                    init                    ();
    virtual void        destroy();

    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLCANVASSIZE_H
