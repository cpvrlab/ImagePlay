#ifndef IPLCOMBINEIMAGES_H
#define IPLCOMBINEIMAGES_H

#include "IPL_global.h"
#include "IPL.h"

#include <string>

/**
 * @brief The IPLCombineImages class
 */
class IPLSHARED_EXPORT IPLCombineImages : public IPLProcess
{
public:
    IPLCombineImages();
    ~IPLCombineImages();
    IPLProcess* clone() { return (IPLProcess*) new IPLCombineImages(*this); }

    void init();

    virtual bool processInputData( IPLImage* data, int inNr );
    virtual IPLImage* getResultData( int outNr );

protected:
    IPLImage*           _result;
    IPLImage*           _inputA;
    IPLImage*           _inputB;
    int                 _operation;
    double              _factorA;
    double              _factorB;
};

#endif // IPLCOMBINEIMAGES_H
