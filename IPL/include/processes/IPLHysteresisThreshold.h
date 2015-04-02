#ifndef IPLHysteresisThreshold_H
#define IPLHysteresisThreshold_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <deque>

/**
 * @brief The IPLHysteresisThreshold class
 */
class IPLSHARED_EXPORT IPLHysteresisThreshold : public IPLClonableProcess<IPLHysteresisThreshold>
{
public:
                            IPLHysteresisThreshold() : IPLClonableProcess() { init(); }
                            ~IPLHysteresisThreshold()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;

    class Pixel {
        public: Pixel( int _x, int _y ){ x=_x; y=_y; }
        int x;
        int y;
    };
    typedef std::deque<Pixel> Queue;

private:
    int trace(int x, int y, IPLImagePlane *plane, IPLImagePlane *image, float lowThreshold);
};

#endif // IPLHysteresisThreshold_H
