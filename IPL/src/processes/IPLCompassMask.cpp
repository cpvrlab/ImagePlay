#include "IPLCompassMask.h"

int const IPLCompassMask::_mask[4][8][3][3] = {

{{ { 1, 1, 1 },    { 1, -2, 1 },  { -1, -1, -1 } },  // Prewitt N
 { { 1, 1, 1 },    { 1, -2, -1 }, { 1, -1, -1 } },   // Prewitt NW
 { { 1, 1, -1 },   { 1, -2, -1 }, { 1, 1, -1 } },    // Prewitt W
 { { 1, -1, -1 },  { 1, -2, -1 }, { 1, 1, 1 } },     // Prewitt SW
 { { -1, -1, -1 }, { 1, -2, 1 },  { 1, 1, 1 } },     // Prewitt S
 { { -1, -1, 1 },  { -1, -2, 1 }, { 1, 1, 1 } },     // Prewitt SE
 { { -1, 1, 1 },   { -1, -2, 1 }, { -1, 1, 1 } },    // Prewitt E
 { { 1, 1, 1 },    { -1, -2, 1 }, { -1, -1, 1 } } }, // Prewitt NE

{{ { 3, 3, 3 },    { 3, 0, 3 },   { -5, -5, -5 } },  // Kirsch N
 { { 3, 3, 3 },    { 3, 0, -5 },  { 3, -5, -5 } },   // Kirsch NW
 { { 3, 3, -5 },   { 3, 0, -5 },  { 3, 3, -5 } },    // Kirsch W
 { { 3, -5, -5 },  { 3, 0, -5 },  { 3, 3, 3 } },     // Kirsch SW
 { { -5, -5, -5 }, { 3, 0, 3 },   { 3, 3, 3 } },     // Kirsch S
 { { -5, -5, 3 },  { -5, 0, 3 },  { 3, 3, 3 } },     // Kirsch SE
 { { -5, 3, 3 },   { -5, 0, 3 },  { -5, 3, 3 } },    // Kirsch E
 { { 3, 3, 3 },    { -5, 0, 3 },  { -5, -5, 3 } } }, // Kirsch NE

{{ { 1, 1, 1 },    { 0, 0, 0 },   { -1, -1, -1 } },  // ThreeLevel N
 { { 1, 1, 0 },    { 1, 0, -1 },  { 0, -1, -1 } },   // ThreeLevel NW
 { { 1, 0, -1 },   { 1, 0, -1 },  { 1, 0, -1 } },    // ThreeLevel W
 { { 0, -1, -1 },  { 1, 0, -1 },  { 1, 1, 0 } },     // ThreeLevel SW
 { { -1, -1, -1 }, { 0, 0, 0 },   { 1, 1, 1 } },     // ThreeLevel S
 { { -1, -1, 0 },  { -1, 0, 1 },  { 0, 1, 1 } },     // ThreeLevel SE
 { { -1, 0, 1 },   { -1, 0, 1 },  { -1, 0, 1 } },    // ThreeLevel E
 { { 0, 1, 1 },    { -1, 0, 1 },  { -1, -1, 0 } } }, // ThreeLevel NE

{{ { 1, 2, 1 },    { 0, 0, 0 },  { -1, -2, -1 } },  // Sobel N
 { { 2, 1, 0 },    { 1, 0, -1 }, { 0, -1, -2 } },   // Sobel NW
 { { 1, 0, -1 },   { 2, 0, -2 }, { 1, 0, -1 } },    // Sobel W
 { { 0, -1, -2 },  { 1, 0, -1 }, { 2, 1, 0 } },     // Sobel SW
 { { -1, -2, -1 }, { 0, 0, 0 },  { 1, 2, 1 } },     // Sobel S
 { { -2, -1, 1 },  { -1, 0, 1 }, { 0, 1, 2 } },     // Sobel SE
 { { -1, 0, 1 },   { -2, 0, 2 }, { -1, 0, 1 } },    // Sobel E
 { { 0, 1, 2 },    { -1, 0, 1 }, { -2, -1, 0 } } }  // Sobel NE
};

void IPLCompassMask::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLCompassMask");
    setTitle("Compass Mask");
    setKeywords("direction");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);
    setDescription("Local compass mask low-pass operator. Prewitt, Kirsch, Sobel, and ThreeLevel masks in all directions.");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("maskType", "Mask Type:Prewitt|Kirsch|ThreeLevel|Sobel", "", IPL_INT_COMBOBOX, 0);
    addProcessPropertyInt("direction", "Direction:N|NW|W|SW|S|SE|E|NE", "", IPL_INT_COMBOBOX, 0);
}

void IPLCompassMask::destroy()
{
    delete _result;
}

bool IPLCompassMask::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

    // get properties
    int maskType = getProcessPropertyInt("maskType");
    int direction = getProcessPropertyInt("direction");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    int startDir = ( direction == 8 )? 0 : direction;
    int endDir = ( direction == 8 )? 7 : direction+1;

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
                    int dirCount = 0;
                    long sum = 0;
                    long total = 0;
                    for( int dir = startDir; dir<endDir; dir++ )
                    {
                        dirCount++;
                        sum = 0;
                        for( int kx=-1; kx<=1; kx++ )
                        {
                            for( int ky=-1; ky<=1; ky++ )
                            {
                                int mask = _mask[maskType][dir][ky+1][kx+1];
                                sum += (long) (plane->cp(x+kx, y+ky) * FACTOR_TO_UCHAR) * (long) mask;
                            }
                        }
                        total += sum;
                    }
                    total /= dirCount;
                    total += 128;
                    total = (total>255)? 255 : (total<0)? 0 : total;
                    newplane->p(x,y) = total * FACTOR_TO_FLOAT;
                }
            }
        }
    }
    return true;
}

IPLData* IPLCompassMask::getResultData( int )
{
    return _result;
}
