//#############################################################################
//
//  This file is part of ImagePlay.
//
//  ImagePlay is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ImagePlay is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with ImagePlay.  If not, see <http://www.gnu.org/licenses/>.
//
//#############################################################################

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
    IPLImagePlane( const IPLImagePlane &other );
    IPLImagePlane( IPLImagePlane &&other );
    IPLImagePlane &operator=(const IPLImagePlane &other);
    IPLImagePlane &operator=(IPLImagePlane &&other);
    ~IPLImagePlane();

    void swap(IPLImagePlane &other);

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
    ipl_basetype*           _plane;
    static ipl_basetype     _zero;
    static int              _instanceCount;
};

#endif // IPLImagePlane_H
