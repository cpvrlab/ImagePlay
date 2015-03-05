#ifndef IPLRANDOMPOINT_H
#define IPLRANDOMPOINT_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLPoint.h"

#include <string>
#include <time.h>

/**
 * @brief The IPLRandomPoint class
 */
class IPLSHARED_EXPORT IPLRandomPoint : public IPLClonableProcess<IPLRandomPoint>
{
public:
    IPLRandomPoint() : IPLClonableProcess() {}
    ~IPLRandomPoint()  { destroy(); }


    void init();
    void destroy();

    virtual bool processInputData( IPLImage* data, int inNr, bool useOpenCV );
    virtual IPLData* getResultData( int outNr );

protected:
    IPLImage*   _image;
    IPLPoint*   _point;
};

#endif // IPLRANDOMPOINT_H
