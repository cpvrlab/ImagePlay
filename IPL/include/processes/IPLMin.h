#ifndef IPLMIN_H
#define IPLMIN_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLMin class
 */
class IPLSHARED_EXPORT IPLMin : public IPLClonableProcess<IPLMin>
{
public:
                            IPLMin() : IPLClonableProcess() { init(); }
                            ~IPLMin()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLMIN_H
