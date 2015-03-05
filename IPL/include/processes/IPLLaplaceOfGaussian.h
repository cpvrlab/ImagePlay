#ifndef IPLLAPLACEOFGAUSSIAN_H
#define IPLLAPLACEOFGAUSSIAN_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief The IPLLaplaceOfGaussian class
 */
class IPLSHARED_EXPORT IPLLaplaceOfGaussian : public IPLClonableProcess<IPLLaplaceOfGaussian>
{
public:
                            IPLLaplaceOfGaussian() : IPLClonableProcess() { init(); }
                            ~IPLLaplaceOfGaussian()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLLAPLACEOFGAUSSIAN_H
