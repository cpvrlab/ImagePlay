#include "IPLNormalizeIllumination.h"

void IPLNormalizeIllumination::init()
{
    // init
    _result         = NULL;
    _illumination   = NULL;
    _cycles = 1;
    _factor = 1;

    // basic settings
    setClassName("IPLNormalizeIllumination");
    setTitle("Normalize Illumination");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);
    addOutput("Illumination Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("cycles", "Cycles", "", 4, IPL_WIDGET_SLIDER, 1, 10);
    addProcessPropertyDouble("factor", "Factor", "", 1.0, IPL_WIDGET_SLIDER, 1.0, 10.0);
}

void IPLNormalizeIllumination::destroy()
{
    delete _result;
    delete _illumination;
}

bool IPLNormalizeIllumination::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;
    delete _illumination;
    _illumination = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage(*image);
    _illumination = new IPLImage( IPLData::IMAGE_GRAYSCALE, width, height );

    // get properties
    int cycles = getProcessPropertyInt("cycles");
    float factor = getProcessPropertyDouble("factor");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    if(image->type() == IPLData::IMAGE_GRAYSCALE || image->type() == IPLData::IMAGE_BW)
    {
        IPLImagePlane* plane = image->plane(0);
        IPLImagePlane* newplane = _result->plane(0);

        normalizePlane(newplane);
    }
/*
    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );
        {
            for(int y=0; y<height; y++)
            {
                // progress
                notifyProgressEventHandler(100*progress++/maxProgress);
                for(int x=0; x<width; x++)
                {
                    newplane->p(x,y) = (plane->p(x,y) < threshold) ? 0.0f : 1.0f;
                }
            }
        }
    }*/
    return true;
}

IPLData* IPLNormalizeIllumination::getResultData(int index)
{
    if(index == 1)
        return _illumination;

    return _result;
}

inline double IPLNormalizeIllumination::min_of( double x1, double x2, double x3 )
{
    return (x1<x2)? ( (x1<x3)? x1 : x3 ) : ( (x2<x3)? x2 : x3 );
}

inline double IPLNormalizeIllumination::max_of( double x1, double x2, double x3 )
{
    return (x1>x2)? ( (x1>x3)? x1 : x3 ) : ( (x2>x3)? x2 : x3 );
}


void IPLNormalizeIllumination::RGBtoHLS( double R, double G, double B, double& H, double& L, double& S )
{
    double maxV = max_of( R, G, B );
    double minV = min_of (R, G, B );
    double diffV = maxV - minV;
    L = (maxV + minV)/2.0;
    if ( fabs( diffV ) == 0.0 )
    {
        H = 0.0;
        S = 0.0;
    }
    else
    {
        if ( L < 0.5 )
            S = diffV/(maxV+minV);
        else
            S = diffV/(2-maxV-minV);
        if( R == maxV )
            H = (G - B)/diffV;
        else if( G == maxV )
            H = 2.0 + (B - R)/diffV;
        else
            H = 4.0 + (R - G)/diffV;
        H /= 6.0;
        if( H < 0.0 ) H += 1.0;
    }
}

double IPLNormalizeIllumination::_RGB( double q1, double q2, double H )
{
    if ( H > 6.0 ) H -= 6.0;
    if ( H < 0.0 ) H += 6.0;
    if( H < 1.0 )
        return q1 + (q2 - q1 ) * H;
    else if( H < 3.0 )
        return q2;
    else if( H < 4.0 )
        return q1 + (q2 - q1 ) * ( 4.0 - H );
    else
        return q1;
}

void IPLNormalizeIllumination::HLStoRGB( double H, double L, double S, double& R, double& G, double& B )
{
     double m2 =  ( L <= 0.5 )? L + L*S : L+S - L*S;
     double m1 = 2.0*L - m2;

     if( S == 0.0 )
     {
         R = L; G = L; B = L;
     }
     else
     {
         double h = H*6.0;
         R = _RGB( m1, m2, h + 2.0 );
         G = _RGB( m1, m2, h );
         B = _RGB( m1, m2, h - 2.0 );
     }
}


void IPLNormalizeIllumination::normalizePlane(IPLImagePlane* plane)
{
    int height = plane->height();
    int width = plane->width();

    int stride = 1;
    for( int cyc=0; cyc<_cycles; ++cyc )
    {
        for( int y=0; y<height; ++y )
        {
            for( int x=0; x<width; ++x )
            {
                double sum = 0.0;
                for( int i=-1*stride; i<=stride; i += stride )
                {
                    for( int j=-1*stride; j<=stride; j+=stride )
                    {
                        sum += plane->bp(x+i,y+j);
                    }
                }
                double avg = sum / 9.0;
                _illumination->plane(0)->p(x,y) = avg;
            }
        }
        stride *= 3;
    }

    double lo = DBL_MAX;
    double hi = DBL_MIN;
    for( int y=0; y<height; ++y )
    {
        for( int x=0; x<width; ++x )
        {
            double result = (_illumination->plane(0)->p(x,y)!=0.0) ? plane->p(x,y) / _illumination->plane(0)->p(x,y) : 1.0;
            plane->p(x,y) = result;
            if( result<lo ) lo = result;
            if( result>hi ) hi = result;
        }
    }

    const int pins = 1024;
    int equ[pins] = {0} ;

    // compute histogram
    double diff = hi - lo;
    for( int x=0; x<width; x++ )
    {
        for( int y=0; y<height; y++ )
        {
            int index = (int)( (plane->p(x,y)-lo)/diff * pins);
            index = (index > pins-1) ? pins-1 : index;
            index = (index < 0) ? 0 : index;
            ++equ[index];
        }
    }
    double hiFact = 0.01;
    double loFact = 0.01;
    int limLo = loFact * (width * height);
    int limHi = hiFact * (width * height);
    int plo = 0; int loSum = 0;
    while( loSum < limLo ) loSum += equ[plo++];
    int phi = pins-1; int hiSum = 0;
    while( hiSum < limHi ) hiSum += equ[phi--];
    double new_lo = lo + (double)plo/(double)pins*diff;
    double new_hi = lo + (double)phi/(double)pins*diff;

    diff = ( new_hi - new_lo );
    for( int y=0; y<height; ++y )
    {
        for( int x=0; x<width; ++x )
        {
            double result = ( plane->p(x,y) - new_lo ) / diff;
            if( result > 1.0 ) result = 1.0;
            if( result < 0.0 ) result = 0.0;
            plane->p(x,y) = result;
        }
    }
}
