#ifndef IPLMarkImage_H
#define IPLMarkImage_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLMarkImage class
 */
class IPLSHARED_EXPORT IPLMarkImage : public IPLClonableProcess<IPLMarkImage>
{
public:
                            IPLMarkImage() : IPLClonableProcess() { init(); }
                            ~IPLMarkImage()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLMarkImage_H
