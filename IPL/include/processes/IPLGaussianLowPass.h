#ifndef IPLGAUSSIANLOWPASS_H
#define IPLGAUSSIANLOWPASS_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief The IPLGaussianLowPass class
 */
class IPLSHARED_EXPORT IPLGaussianLowPass : public IPLClonableProcess<IPLGaussianLowPass>
{
public:
    IPLGaussianLowPass() : IPLClonableProcess() { init(); }
    ~IPLGaussianLowPass()  { destroy(); }

    void                    init                    ();
    virtual void        destroy();

    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
    IPLData*                _kernel;
};

#endif // IPLGAUSSIANLOWPASS_H
