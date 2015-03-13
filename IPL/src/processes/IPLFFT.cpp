#include "IPLFFT.h"

void IPLFFT::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLFFT");
    setTitle("FFT");
    setCategory(IPLProcess::CATEGORY_FOURIER);
    setDescription("Fast Fourier Transform.");

    // inputs and outputs
    addInput("Grayscale Image", IPLData::IMAGE_GRAYSCALE);
    addOutput("Complex Image", IPLImage::IMAGE_COMPLEX);

    // properties
    addProcessPropertyInt("mode", "Windowing Function:None|Hanning|Hamming|Blackman|Border", "", IPL_INT_RADIOBUTTONS, 0);
}

void IPLFFT::destroy()
{
    delete _result;
}

bool IPLFFT::processInputData(IPLImage* image , int, bool useOpenCV)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();

    _result = new IPLComplexImage(width, height);

    // get properties
    float threshold = getProcessPropertyDouble("threshold");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();

    IPLImagePlane* plane = image->plane(0);
    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);
        for(int x=0; x<width; x++)
        {
            _result->real(x,y) = plane->p(x,y);
            _result->imag(x,y) = 0.0f;
        }
    }
    _result->FFT();

    return true;
}

IPLData* IPLFFT::getResultData( int )
{
    return _result;
}
