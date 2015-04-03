#ifndef IPLMORPHOLOGYBINARY_H
#define IPLMORPHOLOGYBINARY_H

#include "IPL_global.h"
#include "IPLProcess.h"

#include <string>

/**
 * @brief The IPLMorphologyBinary class
 */
class IPLSHARED_EXPORT IPLMorphologyBinary : public IPLClonableProcess<IPLMorphologyBinary>
{
public:
    IPLMorphologyBinary() : IPLClonableProcess() { init(); }
    ~IPLMorphologyBinary()  { destroy(); }

    void init();
    virtual void        destroy();

    virtual bool processInputData( IPLImage* data, int inNr, bool useOpenCV );
    virtual IPLImage* getResultData( int outNr );

protected:
    IPLImage*           _result;
    std::vector<int>    _kernel;
    int                 _operation;
    int                 _iterations;
};

#endif // IPLMORPHOLOGYBINARY_H
