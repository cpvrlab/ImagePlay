#ifndef IPLBINARIZEIMAGE_H
#define IPLBINARIZEIMAGE_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/**
 * @brief The IPLBinarizeImage class
 */
class IPLSHARED_EXPORT IPLBinarizeImage : public IPLClonableProcess<IPLBinarizeImage>
{
public:
    IPLBinarizeImage() : IPLClonableProcess() { init(); }
    ~IPLBinarizeImage()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();

    virtual bool            processInputData        (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData           (int outNr);

protected:
    IPLImage*               _result;
};

#endif // IPLBINARIZEIMAGE_H
