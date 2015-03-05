#include "IPLData.h"

#include "IPLImage.h"
#include "IPLPoint.h"
#include "IPLMatrix.h"

IPLImage* IPLData::toImage()
{
    return dynamic_cast<IPLImage*>(this);
}

IPLPoint* IPLData::toPoint()
{
    return dynamic_cast<IPLPoint*>(this);
}

IPLMatrix* IPLData::toMatrix()
{
    return dynamic_cast<IPLMatrix*>(this);
}
