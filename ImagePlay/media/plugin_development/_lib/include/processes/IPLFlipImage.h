#ifndef IPLFLIPIMAGE_H
#define IPLFLIPIMAGE_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/**
 * @brief The IPLFlipImage class
 */
class IPLSHARED_EXPORT IPLFlipImage : public IPLClonableProcess<IPLFlipImage>
{
public:
    IPLFlipImage() : IPLClonableProcess() { init(); }
    ~IPLFlipImage()  { destroy(); }

    void                    init                    ();
    virtual void            destroy();

    virtual bool            processInputData        (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData           (int outNr );

protected:
    IPLImage*               _result;
};

#endif // IPLFLIPIMAGE_H
