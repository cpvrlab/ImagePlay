#include "IPLGabor.h"

void IPLGabor::init()
{
    // init
    _result0    = NULL;
    _result1    = NULL;
    _result2    = NULL;

    // basic settings
    setClassName("IPLGabor");
    setTitle("Gabor Filter");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Even Gabor ", IPLImage::IMAGE_COLOR);
    addOutput("Odd Gabor ", IPLImage::IMAGE_COLOR);
    addOutput("Power Gabor ", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", IPL_INT_SLIDER, 5, 3, 15);
    addProcessPropertyInt("wavelength", "Wavelength", "", IPL_INT_SLIDER, 5, 1, 15);
    addProcessPropertyDouble("direction", "Direction", "", IPL_DOUBLE_SLIDER, 0, 0, 2*PI);
    addProcessPropertyDouble("deviation", "Deviation", "", IPL_DOUBLE_SLIDER, 5, 1, 10);
}

void IPLGabor::destroy()
{
    delete _result0;
    delete _result1;
    delete _result2;
}

bool IPLGabor::processInputData(IPLImage* image , int, bool useOpenCV)
{
    // delete previous result
    delete _result0;
    _result0 = NULL;
    delete _result1;
    _result1 = NULL;
    delete _result2;
    _result2 = NULL;

    int width = image->width();
    int height = image->height();
    _result0 = new IPLImage( image->type(), width, height );
    _result1 = new IPLImage( image->type(), width, height );
    _result2 = new IPLImage( image->type(), width, height );

    // get properties
    int window = getProcessPropertyInt("window");
    int wavelength = getProcessPropertyInt("wavelength");
    double direction = getProcessPropertyDouble("direction");
    double deviation = getProcessPropertyDouble("deviation");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    int w2 = window/2;
    int area = window*window;
    double* qEven = new double [area];
    double* qOdd = new double [area];

    double k = 2.0 * PI / (double) wavelength;
    double k2 = k * k;
    double d2 = deviation * deviation;
    double sig2 = 1.0 / (2.0 * d2);
    double kx = k * cos( direction );
    double ky = -k * sin( direction );

    int index = 0;
    double E = 0.0;
    double O =0.0;
    for( int x = -w2; x <= w2; x++ )
    {
        for( int y = -w2; y <= w2; y++)
        {
            double compensate = k2/d2;
            double envelope = exp( -k2 * sig2 * (x*x+y*y) );
            double DC = exp( -d2/2.0);
            E += qEven[index] = compensate * envelope * ( cos( kx*x + ky*y ) - DC );
            O += qOdd[index++]  = compensate * envelope * ( sin( kx*x + ky*y )- DC );
        }
    }

    for( int planeNr=0; planeNr < image->getNumberOfPlanes(); planeNr++ )
    {
        IPLImagePlane* plane        = image->plane( planeNr );
        IPLImagePlane* evenplane    = _result0->plane( planeNr );
        IPLImagePlane* oddplane     = _result1->plane( planeNr );
        IPLImagePlane* powerplane   = _result2->plane( planeNr );
        for(int x=w2; x<width-w2; x++)
        {
            for(int y=w2; y<height-w2; y++)
            {
                double even = 0;
                double odd = 0;
                double power = 0;
                int i = 0;
                for( int kx=-w2; kx<=w2; kx++ )
                {
                    for( int ky=-w2; ky<=w2; ky++ )
                    {
                        double img = (double) plane->p(x+kx, y+ky);
                        even += img * qEven[i];
                        odd  += img * qOdd[i++];
                    }
                }
                power = (even*even + odd*odd )*2;
                even = even  + 0.5;
                odd = odd + 0.5;
                even = (even>1.0)? 1.0 : (even<0)? 0 : even;
                odd = (odd>1.0)? 1.0 : (odd<0)? 0 : odd;
                power = (power>1.0)? 1.0 : (power<0)? 0 : power;
                evenplane->p(x,y)   =  even;
                oddplane->p(x,y)    = odd;
                powerplane->p(x,y)  =  power;
            }
        }
    }
    delete [] qEven;
    delete [] qOdd;

    return true;
}

IPLData* IPLGabor::getResultData(int index)
{
    if(index == 0)
        return _result0;
    else if(index == 1)
        return _result1;

    return _result2;
}
