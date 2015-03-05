#ifndef IPLTriangleSegmentation_H
#define IPLTriangleSegmentation_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief IPLTriangleSegmentation
 * Segmentation by triangle method.
 */
class IPLSHARED_EXPORT IPLTriangleSegmentation : public IPLClonableProcess<IPLTriangleSegmentation>
{
public:
                            IPLTriangleSegmentation() : IPLClonableProcess() { init(); }
                            ~IPLTriangleSegmentation()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLTriangleSegmentation_H
