#ifndef IPLSYNTHESIZE_H
#define IPLSYNTHESIZE_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLSynthesize class
 */
class IPLSHARED_EXPORT IPLSynthesize : public IPLClonableProcess<IPLSynthesize>
{
public:
    IPLSynthesize() : IPLClonableProcess() { init(); }
    ~IPLSynthesize()  { destroy(); }

    void init();
    virtual void        destroy();
    virtual bool processInputData( IPLImage* data, int inNr, bool useOpenCV );
    virtual IPLImage* getResultData( int outNr );
protected:
    IPLImage*   _result;
    int         _type;
    int         _width;
    int         _height;
    float       _amplitude;
    float       _offset;
    int         _wavelength;
    int         _direction;
    int         _decay;
};


#endif // IPLSYNTHESIZE_H
