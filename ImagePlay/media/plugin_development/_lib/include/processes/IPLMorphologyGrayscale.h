#ifndef IPLMORPHOLOGYGRAYSCALE_H
#define IPLMORPHOLOGYGRAYSCALE_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/**
 * @brief The IPLMorphologyGrayscale class
 */
class IPLSHARED_EXPORT IPLMorphologyGrayscale : public IPLClonableProcess<IPLMorphologyGrayscale>
{
public:
    IPLMorphologyGrayscale() : IPLClonableProcess() { init(); }
    ~IPLMorphologyGrayscale()  { destroy(); }

    void init();
    virtual void        destroy();

    virtual bool processInputData( IPLImage* data, int inNr, bool useOpenCV );
    virtual IPLImage* getResultData( int outNr );

    void dilate(IPLImagePlane* workingPlane, IPLImagePlane* resultPlane);

    uchar max_value(std::vector<uchar> a);

protected:
    IPLImage*           _result;
    std::vector<int>    _kernel;
    int                 _operation;
    int                 _iterations;
};

#endif // IPLMORPHOLOGYGRAYSCALE_H
