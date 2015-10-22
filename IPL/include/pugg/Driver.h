//          Copyright Tunc Bahcecioglu 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <string>

namespace pugg {

class Driver 
{
public:
    Driver(std::string server_name, std::string name, int version)
        : _server_name(server_name), _name(name), _version(version)
    {}
    virtual ~Driver() {}

	std::string server_name() const {return _server_name;}
	std::string name() const {return _name;}
	int version() const {return _version;}

private:
    std::string _name;
    std::string _server_name;
    int _version;
};




}
