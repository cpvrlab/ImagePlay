#ifndef IPLFILLCONCAVITIES_H
#define IPLFILLCONCAVITIES_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>
/**
 * @brief The IPLFillConcavities class
 */
class IPLSHARED_EXPORT IPLFillConcavities : public IPLClonableProcess<IPLFillConcavities>
{
public:
                            IPLFillConcavities() : IPLClonableProcess() { init(); }
                            ~IPLFillConcavities()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLFILLCONCAVITIES_H
