#ifndef IPLRANKTRANSFORM_H
#define IPLRANKTRANSFORM_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLRankTransform class
 */
class IPLSHARED_EXPORT IPLRankTransform : public IPLClonableProcess<IPLRankTransform>
{
public:
                            IPLRankTransform() : IPLClonableProcess() { init(); }
                            ~IPLRankTransform()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLRANKTRANSFORM_H
