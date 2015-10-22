//          Copyright Tunc Bahcecioglu 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <string>
#include <map>
#include <vector>

#include "Driver.h"
#include "Plugin.h"

namespace pugg {

class Kernel;

namespace detail {

template <class K, class V>
void delete_all_values(std::map<K,V>& map) {
    for (typename std::map<K,V>::iterator iter = map.begin(); iter != map.end(); ++iter) {
        delete iter->second;
    }
}
template <class V>
void delete_all_values(std::vector<V>& vec) {
    for (typename std::vector<V>::iterator iter = vec.begin(); iter != vec.end(); ++iter) {
        delete *iter;
    }
}

class Server
{
public:
	Server(std::string name, int min_driver_version) : _name(name), _min_driver_version(min_driver_version) {}
    ~Server() {delete_all_values(_drivers);}

    std::string name() {return _name;}
    int min_driver_version() {return _min_driver_version;}
    std::map<std::string, Driver*>& drivers() {return _drivers;}

	void clear()
	{
		delete_all_values(_drivers);
	}
private:
	std::string _name;
	int _min_driver_version;
	std::map<std::string, Driver*> _drivers;
};
}

class Kernel
{
public:
	virtual ~Kernel()
    {
        pugg::detail::delete_all_values(_servers);
        pugg::detail::delete_all_values(_plugins);
    }

	void add_server(std::string name, int min_driver_version )
	{
        _servers[name] = new pugg::detail::Server(name, min_driver_version);
	}

	bool add_driver(pugg::Driver* driver)
	{
        if (! driver) return false;

        pugg::detail::Server* server = _get_server(driver->server_name());
        if (! server) return NULL;

        if (server->min_driver_version() > driver->version()) return false;

        server->drivers()[driver->name()] = driver;
        return true;
	}

    template <class DriverType>
    DriverType* get_driver(const std::string& server_name, const std::string& name)
    {
        pugg::detail::Server* server = _get_server(server_name);
        if (! server) return NULL;

        std::map<std::string,pugg::Driver*>::iterator driver_iter = server->drivers().find(name);
        if (driver_iter == server->drivers().end()) return NULL;
        return static_cast<DriverType*>(driver_iter->second);
    }

    template <class DriverType>
    std::vector<DriverType*> get_all_drivers(const std::string& server_name)
    {
        std::vector<DriverType*> drivers;

        pugg::detail::Server* server = _get_server(server_name);
        if (! server) return drivers;

        for (std::map<std::string, pugg::Driver*>::iterator iter = server->drivers().begin(); iter != server->drivers().end(); ++iter) {
            drivers.push_back(static_cast<DriverType*>(iter->second));
        }
        return drivers;
    }


	bool load_plugin(const std::string& filename)
	{
        pugg::detail::Plugin* plugin = new pugg::detail::Plugin();
        if (plugin->load(filename)) {
            plugin->register_plugin(this);
            _plugins.push_back(plugin);
            return true;
        } else {
            delete plugin;
            return false;
        }
	}
	
	void clear_drivers()
	{
		for (std::map<std::string,pugg::detail::Server*>::iterator iter = _servers.begin(); iter != _servers.end(); ++iter) {
			iter->second->clear();
		}
	}	
	
	void clear()
	{
        pugg::detail::delete_all_values(_servers);
        pugg::detail::delete_all_values(_plugins);	
	}

protected:
	std::map<std::string,pugg::detail::Server*> _servers;
    std::vector<pugg::detail::Plugin*> _plugins;

    pugg::detail::Server* _get_server(const std::string& name)
    {
        std::map<std::string,pugg::detail::Server*>::iterator server_iter = _servers.find(name);
        if (server_iter == _servers.end())
            return NULL;
        else
            return server_iter->second;
    }
};

}





