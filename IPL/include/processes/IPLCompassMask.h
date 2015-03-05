#ifndef IPLCOMPASSMASK_H
#define IPLCOMPASSMASK_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>

/**
 * @brief The IPLCompassMask class
 */
class IPLSHARED_EXPORT IPLCompassMask : public IPLClonableProcess<IPLCompassMask>
{
public:
                            IPLCompassMask() : IPLClonableProcess() { init(); }
                            ~IPLCompassMask()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

private:
    IPLImage*               _result;
    static int const        _mask[4][8][3][3];
};

#endif // IPLCOMPASSMASK_H
