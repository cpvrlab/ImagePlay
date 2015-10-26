#include <pugg/Kernel.h>

#include "%CLASSNAME%.h"

#ifdef _WIN32
#  define EXPORTIT __declspec( dllexport )
#else
#  define EXPORTIT
#endif

extern "C" EXPORTIT void register_pugg_plugin(pugg::Kernel* kernel)
{
    kernel->add_driver(new %CLASSNAME%Driver());
}
