#ifndef IPLBinarizeEntropy_H
#define IPLBinarizeEntropy_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief IPLBinarizeEntropy
 * Binarize image using maximum sum entropy of classes.
 */
class IPLSHARED_EXPORT IPLBinarizeEntropy : public IPLClonableProcess<IPLBinarizeEntropy>
{
public:
                            IPLBinarizeEntropy() : IPLClonableProcess() { init(); }
                            ~IPLBinarizeEntropy()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLBinarizeEntropy_H
