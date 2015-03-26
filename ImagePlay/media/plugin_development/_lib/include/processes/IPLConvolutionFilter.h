#ifndef IPLCONVOLUTIONFILTER_H
#define IPLCONVOLUTIONFILTER_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/**
 * @brief The IPLConvolutionFilter class
 */
class IPLSHARED_EXPORT IPLConvolutionFilter : public IPLClonableProcess<IPLConvolutionFilter>
{
public:
    IPLConvolutionFilter() : IPLClonableProcess() { init(); }
    ~IPLConvolutionFilter()  { destroy(); }

    void                    init                    ();
    virtual void        destroy();

    virtual bool            processInputData        (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData           (int outNr);

protected:
    IPLImage*               _result;
    std::vector<int>        _kernel;
    float                   _offset;
    int                     _divisor;
    int                     _borders;
    bool                    _normalize;
};

#endif // IPLCONVOLUTIONFILTER_H
