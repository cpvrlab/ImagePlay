#include "IPLData.h"

#include "IPLImage.h"
#include "IPLComplexImage.h"
#include "IPLPoint.h"
#include "IPLMatrix.h"

IPLImage* IPLData::toImage()
{
    return dynamic_cast<IPLImage*>(this);
}

IPLComplexImage* IPLData::toComplexImage()
{
    return dynamic_cast<IPLComplexImage*>(this);
}

IPLPoint* IPLData::toPoint()
{
    return dynamic_cast<IPLPoint*>(this);
}

IPLMatrix* IPLData::toMatrix()
{
    return dynamic_cast<IPLMatrix*>(this);
}
