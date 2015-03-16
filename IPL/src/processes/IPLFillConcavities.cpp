#include "IPLFillConcavities.h"

void IPLFillConcavities::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLFillConcavities");
    setTitle("Fill Concavities");
    setKeywords("threshold");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);
    setDescription("Local fill concavities operator.");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Gray Image", IPLImage::IMAGE_BW);

    // properties
    addProcessPropertyInt("type", "Type:Bright|Dark", "", IPL_INT_RADIOBUTTONS, 0);
    addProcessPropertyInt("iterations", "iterations", "", IPL_INT_SLIDER, 1, 1, 100);
}

void IPLFillConcavities::destroy()
{
    delete _result;
}

bool IPLFillConcavities::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage(*image);

    // get properties
    int type = getProcessPropertyInt("type");
    int iterations = getProcessPropertyInt("iterations");

    int progress = 0;
    int maxProgress = image->width() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    for( int iter=0; iter<iterations; iter++ )
    {
        IPLImage* tempImage = new IPLImage(*_result);

        #pragma omp parallel for
        for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
        {
            IPLImagePlane* plane = tempImage->plane( planeNr );
            IPLImagePlane* newplane = _result->plane( planeNr );
            for(int x=1; x<width-1; x++)
            {
                // progress
                notifyProgressEventHandler(100*progress++/maxProgress);

                for(int y=1; y<height-1; y++)
                {
                    int P[9];
                    P[0] = (plane->p(x+1, y)     * FACTOR_TO_UCHAR);
                    P[1] = (plane->p(x+1, y-1)   * FACTOR_TO_UCHAR);
                    P[2] = (plane->p(x, y-1)     * FACTOR_TO_UCHAR);
                    P[3] = (plane->p(x-1, y-1)   * FACTOR_TO_UCHAR);
                    P[4] = (plane->p(x-1, y)     * FACTOR_TO_UCHAR);
                    P[5] = (plane->p(x-1, y+1)   * FACTOR_TO_UCHAR);
                    P[6] = (plane->p(x, y+1)     * FACTOR_TO_UCHAR);
                    P[7] = (plane->p(x+1, y+1)   * FACTOR_TO_UCHAR);
                    P[8] = (plane->p(x, y)       * FACTOR_TO_UCHAR);

                    if( type == 0 ) // bright
                    {
                        bool found = false;
                        int maxc = 0;
                        for( int r=0; r<8; r++ )
                        {
                            maxc = 0;
                            found = true;
                            for( int s=0; (s<4) && found; s++ )
                            {
                                int z = (r+s)%8;
                                if(P[8] <= P[z]) found = false;
                                if( (P[z] > maxc) && found) maxc = P[z];
                            }
                            if (found)
                                break;
                        }
                        newplane->p(x,y) = (float) (((found)? maxc : P[8]) * FACTOR_TO_FLOAT);
                    }
                    else // dark
                    {
                        bool found = false;
                        int minc = 255;
                        for( int r=0; r<8; r++ )
                        {
                            minc = 255;
                            found = true;
                            for( int s=0; (s<4) && found; s++ )
                            {
                                int z = (r+s)%8;
                                if(P[8] >= P[z]) found = false;
                                if( (P[z] < minc) && found) minc = P[z];
                            }
                            if (found)
                                break;
                        }
                        newplane->p(x,y) = (float) (((found)? minc : P[8]) * FACTOR_TO_FLOAT);
                    }
                }
            }
        }
        delete tempImage;
    }
    return true;
}

IPLData* IPLFillConcavities::getResultData( int )
{
    return _result;
}
