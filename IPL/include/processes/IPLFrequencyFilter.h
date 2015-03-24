#ifndef IPLFREQUENCYFILTER_H
#define IPLFREQUENCYFILTER_H

#include "IPL_global.h"
#include "IPLProcess.h"
#include "IPLComplexImage.h"

#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

/**
 * @brief The IPLFrequencyFilter class
 */
class IPLSHARED_EXPORT IPLFrequencyFilter : public IPLClonableProcess<IPLFrequencyFilter>
{
public:
                            IPLFrequencyFilter() : IPLClonableProcess() { init(); }
                            ~IPLFrequencyFilter()  { destroy(); }

    void                    init                    ();
    void                    destroy                 ();
    bool                    processInputData        (IPLImage*, int, bool useOpenCV);
    IPLData*                getResultData           (int);

protected:
    IPLComplexImage*        _input;
    IPLComplexImage*        _result;
    IPLImage*               _filter;
};

#endif // IPLFREQUENCYFILTER_H
