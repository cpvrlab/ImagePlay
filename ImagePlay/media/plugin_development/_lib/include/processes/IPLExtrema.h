#ifndef IPLEXTREMA_H
#define IPLEXTREMA_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief The IPLExtrema class
 */
class IPLSHARED_EXPORT IPLExtrema : public IPLClonableProcess<IPLExtrema>
{
public:
                            IPLExtrema() : IPLClonableProcess() { init(); }
                            ~IPLExtrema()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLEXTREMA_H
