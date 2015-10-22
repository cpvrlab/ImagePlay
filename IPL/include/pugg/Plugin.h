//          Copyright Tunc Bahcecioglu 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>

#ifdef WIN32
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
    #define NOMINMAX
    #endif
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace pugg {

class Kernel;

namespace detail {
	
typedef void fnRegisterPlugin(pugg::Kernel*);

class DllLoader
{
public:
    ~DllLoader()
    {
        this->free();
    }

    bool load(std::string filename)
    {
#ifdef WIN32
		_handle = LoadLibraryA(filename.c_str());
#else
		_handle = dlopen(filename.c_str(), RTLD_NOW);
#endif
        return (_handle != NULL);
    }
    fnRegisterPlugin* register_function()
    {
#ifdef WIN32
        return reinterpret_cast<fnRegisterPlugin*>(GetProcAddress(_handle, "register_pugg_plugin"));
#else
        return reinterpret_cast<fnRegisterPlugin*>(dlsym(_handle, "register_pugg_plugin"));
#endif
    }
    void free()
    {
#ifdef WIN32
        if (_handle) { FreeLibrary(_handle); }
#else
        if (_handle) { dlclose(_handle); }
#endif
    }
private:
#ifdef WIN32
	HMODULE _handle;
#else
    void* _handle;
#endif
};

class Plugin
{
public:
	Plugin() : _register_function(NULL) {}

	bool load(const std::string& filename)
	{
        if (! _dll_loader.load(filename)) return false;
        _register_function = _dll_loader.register_function();

        if (_register_function) {
            return true;
        } else {
            _dll_loader.free();            
            return false;
        }
    }

	void register_plugin(pugg::Kernel* kernel)
	{
		_register_function(kernel);
	}
private:

	fnRegisterPlugin* _register_function;
    DllLoader _dll_loader;
};



}
}
