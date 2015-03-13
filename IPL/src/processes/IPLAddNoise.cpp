#include "IPLAddNoise.h"

void IPLAddNoise::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLAddNoise");
    setTitle("Add Noise");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);
    setKeywords("gaussian, impulsive");
    setDescription("Add impulse, Gaussian noise (additive or multiplicative) to an image.");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Gray Image", IPLImage::IMAGE_BW);

    // properties
    addProcessPropertyInt("type", "Type:Impulsive|Gaussian", "", IPL_INT_COMBOBOX, 0);
    addProcessPropertyDouble("probability", "Probability", "", IPL_DOUBLE_SLIDER, 0.2, 0.0, 0.99);
    addProcessPropertyDouble("amplitude", "Amplitude", "", IPL_DOUBLE_SLIDER, 0.2, 0.0, 1.0);
    addProcessPropertyDouble("stdDev", "Standard Deviation (Gauss)", "", IPL_DOUBLE_SLIDER, 0.1, 0.0, 1.0);
    addProcessPropertyInt("gaussType", "Type(Gauss):Multiplicative|Additive", "", IPL_INT_RADIOBUTTONS, 0);
}

void IPLAddNoise::destroy()
{
    delete _result;
}

bool IPLAddNoise::processInputData(IPLImage* image , int, bool useOpenCV)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

    // get properties
    int type = getProcessPropertyInt("type");
    float probability = getProcessPropertyDouble("probability");
    float amplitude = getProcessPropertyDouble("amplitude");
    float stdDev = getProcessPropertyDouble("stdDev");
    int gaussType = getProcessPropertyInt("gaussType");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    std::default_random_engine generator;
    generator.seed(time(0));
    std::normal_distribution<double> distribution(1.0, stdDev);

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );
        for(int y=0; y<height; y++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);
            for(int x=0; x<width; x++)
            {
                ipl_basetype p = plane->p(x,y);

                // impulsive noise
                if(type == 0)
                {
                    int qneg = (int)( (double)probability * RAND_MAX) ;
                    int qpos = RAND_MAX - qneg;
                    int r = rand();
                    if( r > qpos ) { p += amplitude; p = (p>1.0) ? 1.0 : p; }
                    if( r < qneg ) { p -= amplitude; p = (p<0.0) ? 0.0 : p; }
                    newplane->p(x,y) = p;
                }
                else
                {
                    // gaussian noise
                    double rand = distribution(generator);

                    if(gaussType == 0) // multiplicative
                    {
                        p *= rand;
                    }
                    else                 // additive
                    {
                        p += rand - 1.0;
                    }
                    p = (p>1.0) ? 1.0 : p;
                    p = (p<0.0) ? 0.0 : p;

                    newplane->p(x,y) = p;
                }
            }
        }
    }
    return true;
}

IPLData* IPLAddNoise::getResultData( int )
{
    return _result;
}
