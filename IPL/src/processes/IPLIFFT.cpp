#include "IPLIFFT.h"

void IPLIFFT::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLIFFT");
    setTitle("Inverse FFT");
    setCategory(IPLProcess::CATEGORY_FOURIER);
    setDescription("Inverse Fast Fourier Transform.");
	setKeywords("IFFT");

    // inputs and outputs
    addInput("Complex Image", IPLData::IMAGE_COMPLEX);
    addOutput("Grayscale Image", IPLImage::IMAGE_GRAYSCALE);

    // properties
    //addProcessPropertyInt("mode", "Windowing Function:None|Hanning|Hamming|Blackman|Border", "", IPL_INT_RADIOBUTTONS, 0);
}

void IPLIFFT::destroy()
{
    delete _result;
}

bool IPLIFFT::processInputData(IPLImage* data , int, bool)
{
    _complexImage = new IPLComplexImage(*data->toComplexImage());

    // delete previous result
    delete _result;
    _result = NULL;

    int width = _complexImage->width();
    int height = _complexImage->height();

    _result = new IPLImage(IPLData::IMAGE_GRAYSCALE, width, height);

    // get properties
    //int mode = getProcessPropertyInt("mode");

    int progress = 0;
    int maxProgress = height;

    _complexImage->IFFT();

    float dc = _complexImage->real(0,0);

    float min = _complexImage->minReal();
    float max = _complexImage->maxReal();

    float diff = max-min;

    for(int y=0; y<height; y++)
    {
        // progress
        notifyProgressEventHandler(100*progress++/maxProgress);
        for(int x=0; x<width; x++)
        {
            float value = (_complexImage->real(x,y) - min) / diff;
            value = (value < 0) ? 0 : value;
            value = (value > 1) ? 1 : value;
            _result->plane(0)->p(x, y) = value;
        }
    }

    std::stringstream s;
    s << "DC: ";
    s << dc;
    addInformation(s.str());

    return true;
}

IPLData* IPLIFFT::getResultData( int )
{
    return _result;
}
