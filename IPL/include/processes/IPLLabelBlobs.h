#ifndef IPLLABELBLOBS_H
#define IPLLABELBLOBS_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>
#include <deque>

/**
 * @brief The IPLLabelBlobs class
 */
class IPLSHARED_EXPORT IPLLabelBlobs : public IPLClonableProcess<IPLLabelBlobs>
{
public:
                            IPLLabelBlobs() : IPLClonableProcess() { init(); }
                            ~IPLLabelBlobs()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
private:
    void                    labelBlob(IPLImagePlane *inPlane, IPLImagePlane *outPlane, int x, int y, float label);

    struct Pixel {
        Pixel( int _x, int _y ){ x=_x; y=_y; };
        int x;
        int y;
    };

    typedef std::deque<IPLLabelBlobs::Pixel> Queue;
};


#endif // IPLLABELBLOBS_H
