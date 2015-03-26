#ifndef IPLCAMERA_H
#define IPLCAMERA_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLCameraIO.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


/**
 * @brief The IPLCamera class
 */
class IPLSHARED_EXPORT IPLCamera : public IPLClonableProcess<IPLCamera>
{
public:
                            IPLCamera() : IPLClonableProcess() { init(); }
                            ~IPLCamera()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    virtual bool            processInputData        (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData           (int outNr);
protected:
    IPLImage*               _result;
};

#endif // IPLCAMERA_H
