#ifndef IPLFFT_H
#define IPLFFT_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLComplexImage.h"

#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

/**
 * @brief The IPLFFT class
 */
class IPLSHARED_EXPORT IPLFFT : public IPLClonableProcess<IPLFFT>
{
public:
                            IPLFFT() : IPLClonableProcess() { init(); }
                            ~IPLFFT()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLComplexImage*        _result;
private:
    double                  Hanning                 (int n, int size);
    double                  Hamming                 (int n, int size);
    double                  Blackman                (int n, int size);
};

#endif // IPLFFT_H
