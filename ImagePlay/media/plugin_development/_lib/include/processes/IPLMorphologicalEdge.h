#ifndef IPLMORPHOLOGICALEDGE_H
#define IPLMORPHOLOGICALEDGE_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLMorphologicalEdge class
 */
class IPLSHARED_EXPORT IPLMorphologicalEdge : public IPLClonableProcess<IPLMorphologicalEdge>
{
public:
                            IPLMorphologicalEdge() : IPLClonableProcess() { init(); }
                            ~IPLMorphologicalEdge()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLMORPHOLOGICALEDGE_H
