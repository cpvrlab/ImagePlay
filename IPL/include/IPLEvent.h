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

#ifndef IPLEVENT_H
#define IPLEVENT_H

#include "IPL_global.h"
#include <string>

/**
 * @brief The IPLEvent class
 */
class IPLEvent
{
public:
    IPLEvent(const char* name)
    {
        _eventType = IPL_EVENT_DEFAULT;
        _eventName = std::string(name);
    }
    IPLEventType type() { return _eventType; }
    std::string name() { return _eventName; }
private:
    IPLEventType _eventType;
    std::string  _eventName;
};

#endif // IPLEVENT_H
