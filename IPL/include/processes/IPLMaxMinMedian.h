#ifndef IPLMAXMINMEDIAN_H
#define IPLMAXMINMEDIAN_H

#include "IPL_global.h"
#include "IPLProcess.h"

/**
 * @brief The IPLMaxMinMedian class
 */
class IPLSHARED_EXPORT IPLMaxMinMedian : public IPLClonableProcess<IPLMaxMinMedian>
{
public:
                            IPLMaxMinMedian() : IPLClonableProcess() { init(); }
                            ~IPLMaxMinMedian()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLImage*               _result;
private:
    ipl_basetype            computeMedian(ipl_basetype *list, int size);
};

#endif // IPLMAXMINMEDIAN_H
