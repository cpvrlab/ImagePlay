#ifndef IPLNegate_H
#define IPLNegate_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLNegate class
 */
class IPLSHARED_EXPORT IPLNegate : public IPLClonableProcess<IPLNegate>
{
public:
                            IPLNegate() : IPLClonableProcess() { init(); }
                            ~IPLNegate()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLNegate_H
