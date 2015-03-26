#ifndef IPLStretchContrast_H
#define IPLStretchContrast_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLStretchContrast class
 */
class IPLSHARED_EXPORT IPLStretchContrast : public IPLClonableProcess<IPLStretchContrast>
{
public:
                            IPLStretchContrast() : IPLClonableProcess() { init(); }
                            ~IPLStretchContrast()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLStretchContrast_H
