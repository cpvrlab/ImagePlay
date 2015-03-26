#ifndef IPLCONVERTTOGRAY_H
#define IPLCONVERTTOGRAY_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * \brief The IPLConvertToGray class.
 *
 * \copydoc ConvertToGray
 */
class IPLSHARED_EXPORT IPLConvertToGray : public IPLClonableProcess<IPLConvertToGray>
{
public:
    IPLConvertToGray() : IPLClonableProcess() { init(); }
    ~IPLConvertToGray()  { destroy(); }
    void                    init();
    virtual void        destroy();
    virtual bool            processInputData        (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*       getResultData           (int outNr);
protected:
    IPLImage*               _result;
};

#endif // IPLCONVERTTOGRAY_H
