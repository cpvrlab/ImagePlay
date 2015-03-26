#ifndef IPLRESIZE_H
#define IPLRESIZE_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLMatrix.h"

#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/**
 * @brief The IPLResize class
 */
class IPLSHARED_EXPORT IPLResize : public IPLClonableProcess<IPLResize>
{
public:
                            IPLResize() : IPLClonableProcess() { init(); }
                            ~IPLResize()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int index, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLRESIZE_H
