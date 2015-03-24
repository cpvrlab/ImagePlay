#ifndef IPLIFFT_H
#define IPLIFFT_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLComplexImage.h"

#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

/**
 * @brief The IPLIFFT class
 */
class IPLSHARED_EXPORT IPLIFFT : public IPLClonableProcess<IPLIFFT>
{
public:
                            IPLIFFT() : IPLClonableProcess() { init(); }
                            ~IPLIFFT()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLComplexImage*        _complexImage;
    IPLImage*               _result;
};

#endif // IPLIFFT_H
