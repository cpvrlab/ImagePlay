#ifndef %CLASSNAME%_H
#define %CLASSNAME%_H

#include "IPL_global.h"
#include "IPLProcess.h"


/*!
 * \ The process
 */
class %CLASSNAME% : public IPLClonableProcess<%CLASSNAME%>
{
public:
    %CLASSNAME%() : IPLClonableProcess() { init(); }
    ~%CLASSNAME%()  { destroy(); }

    void init();
    void destroy();
    bool processInputData(IPLImage* image, int, bool useOpenCV);
    IPLData* getResultData(int);

private:
    IPLImage*  _result;
};

/*!
 * \The plugin container class
 */
class %CLASSNAME%Driver : public IPLProcessDriver
{
public:
    %CLASSNAME%Driver() : IPLProcessDriver("%CLASSNAME%Driver", %CLASSNAME%::version) {}
    IPLProcess* create()    { return new %CLASSNAME%(); }
    std::string className() { return "%CLASSNAME%"; }
    std::string author()    { return "%AUTHOR%"; }
    int         version()   { return 1; }
};


#endif // %CLASSNAME%_H

