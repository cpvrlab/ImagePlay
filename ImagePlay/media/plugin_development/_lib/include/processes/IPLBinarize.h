#ifndef IPLBinarize_H
#define IPLBinarize_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/**
 * @brief The IPLBinarize class
 */
class IPLSHARED_EXPORT IPLBinarize : public IPLClonableProcess<IPLBinarize>
{
public:
    IPLBinarize() : IPLClonableProcess() { init(); }
    ~IPLBinarize()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();

    virtual bool            processInputData        (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData           (int outNr);

protected:
    IPLImage*               _result;
};

#endif // IPLBinarize_H
