#ifndef IPLINVERSECONTRASTRATIOMAPPING_H
#define IPLINVERSECONTRASTRATIOMAPPING_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief The IPLInverseContrastRatioMapping class
 */
class IPLSHARED_EXPORT IPLInverseContrastRatioMapping : public IPLClonableProcess<IPLInverseContrastRatioMapping>
{
public:
                            IPLInverseContrastRatioMapping() : IPLClonableProcess() { init(); }
                            ~IPLInverseContrastRatioMapping()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLINVERSECONTRASTRATIOMAPPING_H
