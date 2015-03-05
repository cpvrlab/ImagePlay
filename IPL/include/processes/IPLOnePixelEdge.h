#ifndef IPLONEPIXELEDGE_H
#define IPLONEPIXELEDGE_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLOnePixelEdge class
 */
class IPLSHARED_EXPORT IPLOnePixelEdge : public IPLClonableProcess<IPLOnePixelEdge>
{
public:
                            IPLOnePixelEdge() : IPLClonableProcess() { init(); }
                            ~IPLOnePixelEdge()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLONEPIXELEDGE_H
