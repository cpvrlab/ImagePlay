#ifndef IPLImagePlane_H
#define IPLImagePlane_H

#include "IPL_global.h"

/**
 * @brief The IPLImagePlane class
 */
class IPLSHARED_EXPORT IPLImagePlane
{
public:
    IPLImagePlane();
    IPLImagePlane( int width, int height );
    IPLImagePlane( const IPLImagePlane& i );
    ~IPLImagePlane();

    //IPLImagePlane& IPLImagePlane::operator=( IPLImagePlane& i );
    //!
    //! \brief pixel access without checks
    //! \param x
    //! \param y
    //! \return
    //!
    ipl_basetype& p( int x, int y )
    {
        return _plane[y * _width + x];
    }

    //!
    //! \brief pixel access with check, zero for invalid coordinates
    //! \param x
    //! \param y
    //! \return
    //!
    ipl_basetype& cp( int x, int y )
    {
        if( x>=0 && x<_width && y>=0 && y<_height )
            return _plane[y * _width + x];
        else
            return _zero;
    }

    //!
    //! \brief pixel access with check, extend border mode
    //! \param x
    //! \param y
    //! \return
    //!
    ipl_basetype& bp( int x, int y )
    {
        if( x<0 ) x = 0;
        if( x >= _width ) x = _width-1;
        if( y<0 ) y = 0;
        if( y >= _height ) y = _height-1;
        return _plane[y * _width + x];
    }

    //!
    //! \brief pixel access with check, wrap border mode
    //! \param x
    //! \param y
    //! \return
    //!
    ipl_basetype& wp( int x, int y )
    {
        if( x<0 ) x = x+_width;
        if( x >= _width ) x = x-_width;
        if( y<0 ) y = y+_height;
        if( y >= _height ) y = y-_height;
        return _plane[y * _width + x];
    }

    int width( void ) { return _width; }
    int height( void ) { return _height; }

private:
    void newPlane( void );
    void deletePlane( void );

    int                     _height;
    int                     _width;
    ipl_basetype*          _plane;
    static ipl_basetype    _zero;
    static int              _instanceCount;
};

#endif // IPLImagePlane_H
