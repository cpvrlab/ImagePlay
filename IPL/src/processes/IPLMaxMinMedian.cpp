#include "IPLMaxMinMedian.h"

void IPLMaxMinMedian::init()
{
    // init
    _result     = NULL;

    // basic settings
    setClassName("IPLMaxMinMedian");
    setTitle("MaxMin Median Operator");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Image", IPLImage::IMAGE_COLOR);

    // properties
    addProcessPropertyInt("window", "Window", "", IPL_INT_SLIDER_ODD, 3, 3, 9);
}

void IPLMaxMinMedian::destroy()
{
    delete _result;
}

bool IPLMaxMinMedian::processInputData(IPLImage* image , int, bool)
{
    // delete previous result
    delete _result;
    _result = NULL;

    int width = image->width();
    int height = image->height();
    _result = new IPLImage( image->type(), width, height );

    // get properties
    int window = getProcessPropertyInt("window");

    int progress = 0;
    int maxProgress = image->height() * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    int w2 = window/2;
    int area = window*window;

    #pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );

        ipl_basetype* list = new ipl_basetype[area];

        for(int x=0; x<width; x++)
        {
            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            ipl_basetype m0, m[4], ma, mi;

            for(int y=0; y<height; y++)
            {
                int i = 0;
                for( int ky=-w2; ky <= w2; ky++ )
                    for( int kx=-w2; kx <= w2; kx++ )
                        list[i++] = plane->bp(x+kx, y+ky);
                m0 = computeMedian( list, area );

                i = 0;
                for( int z=-w2; z <= w2; z++ )
                    list[i++] = plane->bp(x+z, y);
                m[0] = computeMedian( list, 2*w2+1);

                i = 0;
                for( int z=-w2; z <= w2; z++ )
                    list[i++] = plane->bp(x, y+z);
                m[1] = computeMedian( list, 2*w2+1);

                i = 0;
                for( int z=-w2; z <= w2; z++ )
                    list[i++] = plane->bp(x+z, y+z);
                m[2] = computeMedian( list, 2*w2+1);

                i = 0;
                for( int z=-w2; z <= w2; z++ )
                    list[i++] = plane->bp(x-z, y+z);
                m[3] = computeMedian( list, 2*w2+1);

                ma = m[0];
                for( int z=1; z<4; z++ )
                    ma = ( ma > m[z])? ma : m[z];
                mi = m[0];
                for( int z=1; z<4; z++ )
                    mi = ( mi < m[z])? mi : m[z];

                newplane->p(x,y) = ( abs(ma-m0)>=abs(m0-mi) )? ma : mi;
            }
        }
    }
    return true;
}

IPLData* IPLMaxMinMedian::getResultData( int )
{
    return _result;
}


ipl_basetype IPLMaxMinMedian::computeMedian(ipl_basetype* list, int size)
{
    // insert sort list
    for( int k=size; k>=0; k-- )
    {
        int j = k+1;
        ipl_basetype  temp = list[k];
        while( j < size && temp > list[j] )
        {
            list[j-1] = list[j];
            j++;
        }
        list[j-1] = temp;
    }
    return list[size/2];
}
