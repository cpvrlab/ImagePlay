#ifndef IPLBinarizeSavola_H
#define IPLBinarizeSavola_H

#include "IPL_global.h"
#include "IPLProcess.h"
/**
 * @brief The IPLBinarizeSavola class
 *
 * Local Threshold as proposed by Savola et. al.
 */
class IPLSHARED_EXPORT IPLBinarizeSavola : public IPLClonableProcess<IPLBinarizeSavola>
{
public:
                            IPLBinarizeSavola() : IPLClonableProcess() { init(); }
                            ~IPLBinarizeSavola()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLBinarizeSavola_H
