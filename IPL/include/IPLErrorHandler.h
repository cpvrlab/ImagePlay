//#############################################################################
//
//  File:      IPLErrorHandler.h
//  Author:    Kaspar Schmid
//  Date:      November 2014
//
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

#ifndef IPLERRORHANDLER_H
#define IPLERRORHANDLER_H

#include "IPL_global.h"

/**
 * @brief The IPLErrorHandler class
 */
class IPLErrorHandler
{
public:
    IPLErrorHandler()
    {

    }
    virtual void error() = 0;
};

#endif // IPLERRORHANDLER_H
