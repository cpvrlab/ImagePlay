//#############################################################################
//  File:      IPProcessScript.h
//  Author:    Kaspar Schmid
//  Date:      December 2014
//#############################################################################

#ifndef IPPROCESSSCRIPT_H
#define IPPROCESSSCRIPT_H

#include "IPL_global.h"
#include "IPL_processes.h"
#include "IPLProcess.h"

#include <QPushButton>

//-----------------------------------------------------------------------------
//! IPLProcessScript
/*!
 * @todo: currently unused
 */
class IPProcessScript : public IPLClonableProcess<IPProcessScript>
{
public:
    void                init                ();
    void                destroy             ();

    virtual bool        processInputData    (IPLImage* data, int inNr, bool useOpenCV);
    virtual IPLImage*   getResultData       ( int outNr );

protected:
    IPLImage*           _result;
    //QScriptEngine*    _scriptEngine;
};

#endif // IPPROCESSSCRIPT_H
