#ifndef IPLBinarizeOtsu_H
#define IPLBinarizeOtsu_H

#include "IPL_global.h"
#include "IPLProcess.h"
/**
 * @brief The IPLBinarizeOtsu class
 */
class IPLSHARED_EXPORT IPLBinarizeOtsu : public IPLClonableProcess<IPLBinarizeOtsu>
{
public:
                            IPLBinarizeOtsu() : IPLClonableProcess() { init(); }
                            ~IPLBinarizeOtsu()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLBinarizeOtsu_H
