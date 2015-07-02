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

#ifndef IPLOutputsChangedEventHandler_H
#define IPLOutputsChangedEventHandler_H

#include "IPL_global.h"

class IPLProcess;

/**
 * @brief The IPLOutputsChangedEventHandler class
 */
class IPLOutputsChangedEventHandler
{
public:
    IPLOutputsChangedEventHandler()
    {

    }
    virtual void outputsChanged(IPLProcess*) = 0;
};

#endif // IPLOutputsChangedEventHandler_H
