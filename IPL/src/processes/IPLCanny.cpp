#include "IPLCanny.h"

void IPLCanny::init()
{
    // init
    _result         = NULL;
    _binaryImage    = NULL;
    _orientedImage  = NULL;

    // basic settings
    setClassName("IPLCanny");
    setTitle("Canny Edge Detector");
    setCategory(IPLProcess::CATEGORY_LOCALOPERATIONS);
    setOpenCVSupport(IPLOpenCVSupport::OPENCV_ONLY);
    setDescription("The Canny edge detector delivers the magnitude and the gradient of the edge "
                   "image. Thresholding has to be done by an appropriate operator. The σ value "
                   "is used for both, smoothing and derivation operation.");

    // inputs and outputs
    addInput("Image", IPLData::IMAGE_COLOR);
    addOutput("Magnitude", IPLImage::IMAGE_GRAYSCALE);
    addOutput("Edge", IPLImage::IMAGE_GRAYSCALE);
    addOutput("Gradient", IPLImage::IMAGE_GRAYSCALE);

    // properties
    addProcessPropertyInt("window", "Window", "", IPL_INT_SLIDER, 3,3,9);
    addProcessPropertyDouble("sigma", "Sigma", "", IPL_DOUBLE_SLIDER, 1.5, 0.5, 10);
}

void IPLCanny::destroy()
{
    //delete _result;
}

double IPLCanny::gauss( double x, double ssq )
{
    double z;
    if (ssq == 0) return 0.0;
    z = (double)exp( ((-x*x)/(2.0*ssq)) )/(2.0*PI*ssq);
    return z;
}

double IPLCanny::dGauss ( double x, double /*y*/, double ssq )
{
    return -x/(ssq) * gauss(x, ssq);
//	return -x*exp(-(x*x+y*y)/(2.0*ssq))/(M_PI*ssq);
}

double IPLCanny::Norm ( double x, double y )
{
    return sqrt ( (x*x + y*y) );
}

void IPLCanny::trace( int x, int y, double lowThreshold, IPLOrientedImage* dI, IPLImagePlane* image )
{
    int width = dI->width();
    int height = dI->height();
    Queue queue;
    queue.push_back( Pixel( x,y ) );
    while( ! queue.empty() )
    {
        Pixel c = queue.front(); queue.pop_front();
        if( c.x<width && c.x >=0 && c.y<height && c.y>=0 )
        {
            if( dI->magnitude(c.x,c.y)>lowThreshold && (! image->p(c.x,c.y)) )
            {
                image->p(c.x,c.y) = 255;
                queue.push_back( Pixel( c.x+1, c.y ) );
                queue.push_back( Pixel( c.x, c.y+1 ) );
                queue.push_back( Pixel( c.x-1, c.y ) );
                queue.push_back( Pixel( c.x, c.y-1 ) );
                queue.push_back( Pixel( c.x+1, c.y+1 ) );
                queue.push_back( Pixel( c.x-1, c.y+1 ) );
                queue.push_back( Pixel( c.x-1, c.y-1 ) );
                queue.push_back( Pixel( c.x+1, c.y-1 ) );
            }
        }
    }
}


void IPLCanny::thinning(IPLOrientedImage* dI, IPLImagePlane* image, IPLImagePlane* newplane )
{
    static int weights3[3][3] = {1, 8, 64, 2, 16, 128, 4, 32, 256};
    static unsigned char lut1[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    static unsigned char lut2[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,1,1,0,1,1,1};

    int width = image->width();
    int height = image->height();
    IPLImagePlane localImage( width, height );
    for(int x=0; x<width; x++)
    {
        for(int y=0; y<height; y++)
        {
            if( image->bp(x,y) )
            {
                int lutOffset = 0;
                for( int i=-1; i<=1; ++i )
                {
                    for( int j=-1; j<=1; ++j )
                    {
                        if( image->bp(x+i,y+j) )
                            lutOffset += weights3[i+1][j+1];
                    }
                }
                localImage.bp(x,y) = ( lut1[lutOffset] )? 255 : 0;
            }
        }
    }

    for(int x=0; x<width; x++)
    {
        for(int y=0; y<height; y++)
        {
            if( localImage.bp(x,y) )
            {
                int lutOffset = 0;
                for( int i=-1; i<=1; ++i )
                {
                    for( int j=-1; j<=1; ++j )
                    {
                        if( localImage.bp(x+i,y+j) )
                            lutOffset += weights3[i+1][j+1];
                    }
                }
                image->bp(x,y) = ( lut1[lutOffset] )? 255 : 0;
                if( ! lut1[lutOffset] )
                {
                    dI->magnitude(x,y) = 0.0;
                    newplane->bp(x,y) = 0;
                }
            }
            else
            {
                image->bp(x,y) = 0;
                dI->magnitude(x,y) = 0.0;
                newplane->bp(x,y) = 0;
            }

        }
    }
}

bool IPLCanny::processInputData(IPLImage* image , int, bool useOpenCV)
{
    // delete previous result
    delete _result;
    _result = NULL;
    delete _binaryImage;
    _binaryImage = NULL;

    int width = image->width();
    int height = image->height();

    _result = new IPLImage( image->type(), width, height );
    _binaryImage = new IPLImage( IPLData::IMAGE_BW, width, height );

    // get properties
    int window      = getProcessPropertyInt("window");
    double sigma    = getProcessPropertyDouble("sigma");

    if(useOpenCV)
    {
        cv::Mat input;
        cv::Mat output;
        cvtColor(image->toCvMat(), input, CV_BGR2GRAY);
        cv::Canny(input, output, sigma*128, sigma*255, window);

        delete _result;
        _result = new IPLImage(output);

        return true;
    }

    // Create a Gaussian 1D filter
    int N = ceil( sigma * sqrt( 2.0*log( 1.0/0.015 ) ) + 1.0 );
    window = 2*N+1;
    double ssq = sigma*sigma;
    double* gau = new double [window];
    double* dgau = new double [window];
    for( int k = -N; k <= N; ++k )
    {
        gau[k+N] = gauss ( (double)k, ssq );
        dgau[k+N] = dGauss ( (double)k, 0, ssq );
    }

    // Create a directional derivative of 2D Gaussian (along X-axis)
    // Since the result is symmetric along X, we can get the derivative along
    // Y-axis simply by transposing the result for X direction.
//		DoubleImage* dgau = new DoubleImage( window, window );
//		for( int y = -N; y <= N; ++y )
//			for( int x = -N; x <= N; ++x )
//				dgau->f(x+N, y+N) = dGauss( x, y, ssq );

    int progress = 0;
    int maxProgress = width * image->getNumberOfPlanes();
    int nrOfPlanes = image->getNumberOfPlanes();

    //#pragma omp parallel for
    for( int planeNr=0; planeNr < nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = image->plane( planeNr );
        IPLImagePlane* newplane = _result->plane( planeNr );

// ******** Gaussian filtering of input image
        IPLImagePlane* gI = new IPLImagePlane( width, height );

        // horizontal run (normalizing original image)
        IPLImagePlane* tmpI = new IPLImagePlane( width, height );
        for(int x=0; x<width; x++)
        {            // progress
            notifyProgressEventHandler(100*progress++/maxProgress);

            for(int y=0; y<height; y++)
            {
                double sum = 0;
                int i = 0;
                for( int kx=-N; kx<=N; kx++ )
                {
                        double img = (double) plane->bp(x+kx, y);
                        sum += (img * gau[i++]);
                }
                tmpI->p(x,y) = (double) (sum);
            }
        }
        // vertiacl run
        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                double sum = 0;
                int i = 0;
                for( int ky=-N; ky<=N; ky++ )
                {
                        double img = tmpI->bp(x, y+ky);
                        sum += (img * gau[i++]);
                }
                gI->p(x,y) = sum;
            }
        }
        //delete tmpI;

// ******** Apply directional derivatives ...

        // ... in x-direction
        IPLImagePlane* dx = new IPLImagePlane( width, height );
        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                dx->p(x,y) = 0.0;
                for( int k=1; k<N; k++ )
                {
                    dx->p(x,y) += ( gI->bp(x-k,y) - gI->bp(x+k,y) ) * dgau[k];
                }
            }
        }
//			double maxVal = 0.0;
//			for(int x=0; x<width; x++)
//				for(int y=0; y<height; y++)
//					if( dx->f(x,y) > maxVal ) maxVal = dx->f(x,y);

        // ... in y-direction
        IPLImagePlane* dy = new IPLImagePlane( width, height );
        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                dy->p(x,y) = 0.0;
                for( int k=1; k<N; k++ )
                {
                    dy->p(x,y) += ( gI->bp(x,y-k) - gI->bp(x,y+k) ) * dgau[k];
                }
            }
        }

// ******** Compute magnitude and binarization thresholds
        IPLImagePlane* mag = new IPLImagePlane( width, height );
        double magMax = 0.0;
        double magMin = 999999999.0;
        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                double val = sqrt( dx->p(x,y)*dx->p(x,y) + dy->p(x,y)*dy->p(x,y) );
                mag->p(x,y) = val;
                if( val > magMax ) magMax = val;
                if( val < magMin ) magMin = val;
            }
        }

//// ******** Non-maxima suppression - edge pixels should be a local maximum
        _orientedImage = new IPLOrientedImage( width, height );
        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                double ix = dx->p(x,y);
                double iy = dy->p(x,y);
                double g = mag->p(x,y);

                // determine 4-neighbor direction of gradient
                int dir4 = 0;
                if( (iy<=0.0 && ix>-iy) || (iy>=0.0 && ix<-iy) )
                    dir4 = 1;
                else if( (ix>0.0 && -iy>=ix) || (ix<0.0 && -iy<=ix) )
                    dir4 = 2;
                else if( (ix<=0.0 && ix>iy) || (ix>=0.0 && ix<iy) )
                    dir4 = 3;
                else if( (iy<0.0 && ix<=iy) || (iy>0.0 && ix>=iy) )
                    dir4 = 4;
                else
                    continue;

                double gradmag1, gradmag2, d;
                switch(dir4)
                {
                    case 1: d = std::fabs(iy/ix);
                            gradmag1 = mag->bp(x+1,y)*(1-d) + mag->bp(x+1,y-1)*d;
                            gradmag2 = mag->bp(x-1,y)*(1-d) + mag->bp(x-1,y+1)*d;
                            break;
                    case 2: d = std::fabs(ix/iy);
                            gradmag1 = mag->bp(x,y-1)*(1-d) + mag->bp(x+1,y-1)*d;
                            gradmag2 = mag->bp(x,y+1)*(1-d) + mag->bp(x-1,y+1)*d;
                            break;
                    case 3: d = std::fabs(ix/iy);
                            gradmag1 = mag->bp(x,y-1)*(1-d) + mag->bp(x-1,y-1)*d;
                            gradmag2 = mag->bp(x,y+1)*(1-d) + mag->bp(x+1,y+1)*d;
                            break;
                    case 4: d = std::fabs(iy/ix);
                            gradmag1 = mag->bp(x-1,y)*(1-d) + mag->bp(x-1,y-1)*d;
                            gradmag2 = mag->bp(x+1,y)*(1-d) + mag->bp(x+1,y+1)*d;
                            break;
                }

                if( g > gradmag1 && g > gradmag2 )
                {
                    _orientedImage->magnitude(x,y) = g;
                    _orientedImage->phase(x,y) = atan2(iy,ix);
                }
            }

        }


        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                _orientedImage->magnitude(x,y) /= magMax;
                double val = _orientedImage->magnitude(x,y)*255.0;
//					double val = mag->f(x,y)/magMax*255.0;
                if (val > 255.0 ) val = 255.0;
                if (val < 0.0 ) val = 0.0;
                newplane->p(x,y) = (unsigned char ) val;
            }
        }

// ******** Binarize with hysteresis threshold
        double hist[ 256 ];
        for( int i=0; i<256; ++i )
            hist[i] = 0;
        int pixCount = 0;
        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                if( _orientedImage->magnitude(x,y) > 0.0 )
                {
                    int index = floor( _orientedImage->magnitude(x,y)*256.0+0.5 );
                    ++hist[ index ];
                    ++pixCount;
                }
            }
        }
        double PercentOfPixelsNotEdges = 0.7*pixCount;
        double highThresh = 0.0;
        double cumsum = 0.0;
        for( int i=0; i<256; ++i )
        {
            cumsum += hist[i];
            if( cumsum > PercentOfPixelsNotEdges )
            {
                highThresh = (double)i / 256.0;
                break;
            }
        }
        double lowThresh = 0.4 * highThresh;
        IPLImagePlane* binPlane = _binaryImage->plane( 0 );
        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                if(_orientedImage->magnitude(x,y) >= highThresh)
                    trace(x, y, lowThresh, _orientedImage, binPlane);
            }
        }
        //delete dx;
        //delete dy;
        //delete gI;

        thinning(_orientedImage, binPlane, newplane );
    }

    //delete [] gau;
    //delete [] dgau;

    return true;
}

IPLData* IPLCanny::getResultData( int )
{
    return _result;
}
