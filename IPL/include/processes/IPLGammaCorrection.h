#ifndef IPLGAMMACORRECTION_H
#define IPLGAMMACORRECTION_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/**
 * @brief The IPLGammaCorrection class
 */
class IPLSHARED_EXPORT IPLGammaCorrection : public IPLClonableProcess<IPLGammaCorrection>
{
public:
    IPLGammaCorrection() : IPLClonableProcess() { init(); }
    ~IPLGammaCorrection()  { destroy(); }

    void                    init                        ();
    virtual void        destroy();

    virtual bool            processInputData            (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData               (int outNr);

protected:
    IPLImage*   _result;
    float       _gamma;
};

#endif // IPLGAMMACORRECTION_H
