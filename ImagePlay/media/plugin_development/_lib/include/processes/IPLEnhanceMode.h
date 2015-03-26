#ifndef IPLENHANCEMODE_H
#define IPLENHANCEMODE_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>

/**
 * @brief The IPLEnhanceMode class
 */
class IPLSHARED_EXPORT IPLEnhanceMode : public IPLClonableProcess<IPLEnhanceMode>
{
public:
                            IPLEnhanceMode() : IPLClonableProcess() { init(); }
                            ~IPLEnhanceMode()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLENHANCEMODE_H
