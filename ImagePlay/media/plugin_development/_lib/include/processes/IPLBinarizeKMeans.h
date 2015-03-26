#ifndef IPLBinarizeKMeans_H
#define IPLBinarizeKMeans_H

#include "IPL_global.h"
#include "IPLProcess.h"
/**
 * @brief The IPLBinarizeKMeans class
 */
class IPLSHARED_EXPORT IPLBinarizeKMeans : public IPLClonableProcess<IPLBinarizeKMeans>
{
public:
                            IPLBinarizeKMeans() : IPLClonableProcess() { init(); }
                            ~IPLBinarizeKMeans()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLBinarizeKMeans_H
