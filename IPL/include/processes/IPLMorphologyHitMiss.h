#ifndef IPLMORPHOLOGYHITMISS_H
#define IPLMORPHOLOGYHITMISS_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/**
 * @brief The IPLMorphologyHitMiss class
 */
class IPLSHARED_EXPORT IPLMorphologyHitMiss : public IPLClonableProcess<IPLMorphologyHitMiss>
{
public:
    IPLMorphologyHitMiss() : IPLClonableProcess() { init(); }
    ~IPLMorphologyHitMiss()  { destroy(); }

    void init();
    virtual void        destroy();

    virtual bool processInputData( IPLImage* data, int inNr, bool useOpenCV );
    virtual IPLImage* getResultData( int outNr );

    void hitmiss(IPLImagePlane* inputPlane, IPLImagePlane* resultPlane);

protected:
    IPLImage*           _result;
    std::vector<int>    _kernel;
    int                 _progress;
    int                 _maxProgress;
};

#endif // IPLMORPHOLOGYHITMISS_H
