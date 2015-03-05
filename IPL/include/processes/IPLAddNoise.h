#ifndef IPLAddNoise_H
#define IPLAddNoise_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>
#include <random>

/**
 * @brief The IPLAddNoise class
 */
class IPLSHARED_EXPORT IPLAddNoise : public IPLClonableProcess<IPLAddNoise>
{
public:
                            IPLAddNoise() : IPLClonableProcess() { init(); }
                            ~IPLAddNoise()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLAddNoise_H
