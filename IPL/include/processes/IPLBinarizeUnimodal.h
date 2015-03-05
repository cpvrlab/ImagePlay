#ifndef IPLBinarizeUnimodal_H
#define IPLBinarizeUnimodal_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLBinarizeUnimodal class
 */
class IPLSHARED_EXPORT IPLBinarizeUnimodal : public IPLClonableProcess<IPLBinarizeUnimodal>
{
public:
                            IPLBinarizeUnimodal() : IPLClonableProcess() { init(); }
                            ~IPLBinarizeUnimodal()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLBinarizeUnimodal_H
