#ifndef IPLUNSHARPMASKING_H
#define IPLUNSHARPMASKING_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLUnsharpMasking class
 */
class IPLSHARED_EXPORT IPLUnsharpMasking : public IPLClonableProcess<IPLUnsharpMasking>
{
public:
                            IPLUnsharpMasking() : IPLClonableProcess() { init(); }
                            ~IPLUnsharpMasking()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
};

#endif // IPLUNSHARPMASKING_H
