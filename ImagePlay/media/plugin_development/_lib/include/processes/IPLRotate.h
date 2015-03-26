#ifndef IPLROTATE_H
#define IPLROTATE_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief The IPLRotate class
 */
class IPLSHARED_EXPORT IPLRotate : public IPLClonableProcess<IPLRotate>
{
public:
                            IPLRotate() : IPLClonableProcess() { init(); }
                            ~IPLRotate()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLROTATE_H
