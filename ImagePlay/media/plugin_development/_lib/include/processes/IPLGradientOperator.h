#ifndef IPLGRADIENTOPERATOR_H
#define IPLGRADIENTOPERATOR_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLOrientedImage.h"

#include <string>

class IPLOrientedImage;

/**
 * @brief The IPLFlipImage class
 */
class IPLSHARED_EXPORT IPLGradientOperator : public IPLClonableProcess<IPLGradientOperator>
{
public:
    IPLGradientOperator() : IPLClonableProcess() { init(); }
    ~IPLGradientOperator()  { destroy(); }

    void                    init();
    virtual void        destroy();

    virtual bool            processInputData            (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData               (int outNr);

protected:
    IPLOrientedImage*   _result;
};

#endif // IPLGRADIENTOPERATOR_H
