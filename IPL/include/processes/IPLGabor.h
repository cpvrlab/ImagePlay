#ifndef IPLGABOR_H
#define IPLGABOR_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief The IPLGabor class
 */
class IPLSHARED_EXPORT IPLGabor : public IPLClonableProcess<IPLGabor>
{
public:
                            IPLGabor() : IPLClonableProcess() { init(); }
                            ~IPLGabor()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result0;
    IPLImage*               _result1;
    IPLImage*               _result2;
};

#endif // IPLGABOR_H
