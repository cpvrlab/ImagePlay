#ifndef IPLAGGLOMERATE_H
#define IPLAGGLOMERATE_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief The IPLAgglomerate class
 */
class IPLSHARED_EXPORT IPLAgglomerate : public IPLClonableProcess<IPLAgglomerate>
{
public:
                            IPLAgglomerate() : IPLClonableProcess() { init(); }
                            ~IPLAgglomerate()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLAGGLOMERATE_H
