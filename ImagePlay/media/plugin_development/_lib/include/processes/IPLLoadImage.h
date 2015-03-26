#ifndef IPLLOADIMAGE_H
#define IPLLOADIMAGE_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLFileIO.h"

/**
 * @brief The IPLLoadImage class
 */
class IPLSHARED_EXPORT IPLLoadImage : public IPLClonableProcess<IPLLoadImage>
{
public:
    IPLLoadImage() : IPLClonableProcess() { init(); }
    ~IPLLoadImage()  { destroy(); }

    void                init();
    virtual void        destroy();
    virtual bool        processInputData    (IPLImage* data, int index, bool useOpenCV);
    virtual IPLImage*   getResultData       (int outNr);
protected:
    IPLImage*           _result;
    std::string         _path;
};

#endif // IPLLOADIMAGE_H
