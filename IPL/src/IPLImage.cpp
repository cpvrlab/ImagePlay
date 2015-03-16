#include "IPLImage.h"

int IPLImage::_instanceCount = 0;

IPLImage::IPLImage() : IPLData(IPLData::UNDEFINED)
{
    _width = 0;
    _height = 0;
    _nrOfPlanes = 0;
    _rgb32 = NULL;

    _instanceCount++;
}

IPLImage::IPLImage( const IPLImage& other )
{
    _type = other._type;
    _width = other._width;
    _height = other._height;
    _rgb32 = NULL;
    if( _type == IMAGE_COLOR ) _nrOfPlanes = 3; else _nrOfPlanes = 1;
    for( int i=0; i<_nrOfPlanes; i++ )
        _planes.push_back(new IPLImagePlane( *(other._planes[i]) ));

    _instanceCount++;
}

IPLImage::IPLImage( IPLData::IPLDataType t, int width, int height )
{
    _type = t;
    _width = width;
    _height = height;
    _rgb32 = NULL;
    if( _type == IMAGE_COLOR ) _nrOfPlanes = 3; else _nrOfPlanes = 1;
    for( int i=0; i<_nrOfPlanes; i++ )
        _planes.push_back(new IPLImagePlane( width, height ));
    fillColor( 0 );

    _instanceCount++;
}

IPLImage::IPLImage(cv::Mat &cvMat)
{
    // _type = other._type;
    _rgb32  = NULL;
    _width  = cvMat.cols;
    _height = cvMat.rows;
    _type   = IMAGE_COLOR;
    _nrOfPlanes = cvMat.channels();
    if(_nrOfPlanes > 1)
        _type   = IMAGE_COLOR;
    else
        _type   = IMAGE_GRAYSCALE;

    if(_nrOfPlanes > 3)
        _nrOfPlanes = 3;

    for( int i=0; i<_nrOfPlanes; i++ )
    {
        _planes.push_back(new IPLImagePlane(_width, _height));
    }

    if(cvMat.channels() == 1)
    {
        cvMat.convertTo(cvMat, CV_8UC1);

        for(int y = 0; y<_height; y++)
        {
            for(int x = 0; x<_width; x++)
            {
                _planes[0]->p(x,y) = cvMat.at<uchar>(y,x) * FACTOR_TO_FLOAT;
            }
        }
    }
    else if(cvMat.channels() == 2)
    {
        cvMat.convertTo(cvMat, CV_8UC2);

        for(int y = 0; y<_height; y++)
        {
            for(int x = 0; x<_width; x++)
            {
                _planes[0]->p(x,y) = cvMat.at<cv::Vec2b>(y,x).val[0] * FACTOR_TO_FLOAT;
                _planes[1]->p(x,y) = cvMat.at<cv::Vec2b>(y,x).val[1] * FACTOR_TO_FLOAT;
            }
        }
    }
    else if(cvMat.channels() == 3)
    {
        cvMat.convertTo(cvMat, CV_8UC3);

        for(int y = 0; y<_height; y++)
        {
            for(int x = 0; x<_width; x++)
            {
                _planes[0]->p(x,y) = cvMat.at<cv::Vec3b>(y,x).val[2] * FACTOR_TO_FLOAT;   // R
                _planes[1]->p(x,y) = cvMat.at<cv::Vec3b>(y,x).val[1] * FACTOR_TO_FLOAT;   // G
                _planes[2]->p(x,y) = cvMat.at<cv::Vec3b>(y,x).val[0] * FACTOR_TO_FLOAT;   // B
            }
        }
    }
    else if(cvMat.channels() == 4)
    {
        cvMat.convertTo(cvMat, CV_8UC4);

        for(int y = 0; y<_height; y++)
        {
            for(int x = 0; x<_width; x++)
            {
                _planes[0]->p(x,y) = cvMat.at<cv::Vec4b>(y,x).val[2] * FACTOR_TO_FLOAT;   // R
                _planes[1]->p(x,y) = cvMat.at<cv::Vec4b>(y,x).val[1] * FACTOR_TO_FLOAT;   // G
                _planes[2]->p(x,y) = cvMat.at<cv::Vec4b>(y,x).val[0] * FACTOR_TO_FLOAT;   // B
            }
        }
    }

    _instanceCount++;
}

IPLImage::~IPLImage()
{
    delete _rgb32;
    for( int i=0; i<_nrOfPlanes; i++ )
        if( _planes[i] ) delete _planes[i];

    _instanceCount--;
}

IPLImagePlane* IPLImage::plane(int planeNr)
{
    if(planeNr >= (int)_planes.size() || planeNr < 0)
    {
        std::stringstream error("Invalid plane number: ");
        error << planeNr;
        throw std::runtime_error(error.str());
        return NULL;
    }

    return _planes[planeNr];
}

void IPLImage::fillColor(ipl_basetype value)
{
    for( int planeNr = 0; planeNr < _nrOfPlanes; planeNr++ )
    {
        IPLImagePlane* plane = _planes[planeNr];
        for( int x=0; x<_width; x++ )
            for( int y=0; y<_height; y++ )
                plane->p(x,y) = value;
    }
}

std::string IPLImage::toString(int x, int y)
{
    std::stringstream s;
    s << plane(0)->cp(x, y);
    for(int i=1; i<_nrOfPlanes; i++)
    {
        s << ", " << plane(i)->cp(x, y);
    }


    return s.str();
}

cv::Mat IPLImage::toCvMat()
{
    cv::Mat mat(_height, _width, CV_8UC4, rgb32());

    // slower float version
    /*
    cv::Mat mat(_height, _width, CV_32FC3);
    for(int y = 0; y<_height; y++)
    {
        for(int x = 0; x<_width; x++)
        {
            float r = _planes[0]->p(x,y);
            float g = _planes[1]->p(x,y);
            float b = _planes[2]->p(x,y);

            cv::Vec3f value;
            value.val[0] = b;
            value.val[1] = g;
            value.val[2] = r;

            mat.at<cv::Vec3f>(y, x) = value;
        }
    }*/

    return mat;
}

uchar* IPLImage::rgb32() {
    delete _rgb32;
    _rgb32 = new uchar[_height*_width*4];
    if(_type == IMAGE_BW)
    {
        int i=0;
        for(int y=0; y < _height; y++)
        {
            for(int x=0; x < _width; x++)
            {
                uchar val = plane(0)->p(x,y) * FACTOR_TO_UCHAR;
                val = val < 0x80 ? 0x00 : 0xFF;
                _rgb32[i++] = val;
                _rgb32[i++] = val;
                _rgb32[i++] = val;
                _rgb32[i++] = 0xFF;
            }
        }
    }
    else if(_type == IMAGE_GRAYSCALE)
    {
        int i=0;
        for(int y=0; y < _height; y++)
        {
            for(int x=0; x < _width; x++)
            {
                uchar val = (plane(0)->p(x,y) * FACTOR_TO_UCHAR);
                _rgb32[i++] = val;
                _rgb32[i++] = val;
                _rgb32[i++] = val;
                _rgb32[i++] = 0xFF;
            }
        }
    }
    else if(_type == IMAGE_COLOR)
    {
        int i=0;
        for(int y=0; y < _height; y++)
        {
            for(int x=0; x < _width; x++)
            {
                uchar r = plane(0)->p(x,y) * FACTOR_TO_UCHAR;
                uchar g = plane(1)->p(x,y) * FACTOR_TO_UCHAR;
                uchar b = plane(2)->p(x,y) * FACTOR_TO_UCHAR;
                _rgb32[i++] = b;
                _rgb32[i++] = g;
                _rgb32[i++] = r;
                _rgb32[i++] = 0xFF;
            }
        }
    }
    else if(_type == IMAGE_ORIENTED)
    {
        double maxMag = 0.0;
        for(int x=0; x<_width; x++)
            for(int y=0; y<_height; y++)
                if( plane(0)->p(x,y) > maxMag ) maxMag = plane(0)->p(x,y);

        int i=0;
        for(int y=0; y < _height; y++)
        {
            for(int x=0; x < _width; x++)
            {
                ipl_basetype phase = fmod(plane(1)->p(x,y), 1.0);
                ipl_basetype magnitude = fmod(plane(0)->p(x,y) / maxMag, 0.9);

                if(phase < 0)
                    phase = 0;
                if(phase > 1)
                    phase = 1;

                IPLColor color = IPLColor::fromHSV(phase, 1.0,  magnitude);

                uchar r = color.red()   * FACTOR_TO_UCHAR;
                uchar g = color.green() * FACTOR_TO_UCHAR;
                uchar b = color.blue()  * FACTOR_TO_UCHAR;
                _rgb32[i++] = b;
                _rgb32[i++] = g;
                _rgb32[i++] = r;
                _rgb32[i++] = 0xFF;
            }
        }
    }
    return _rgb32;
}
