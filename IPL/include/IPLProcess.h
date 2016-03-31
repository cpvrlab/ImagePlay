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

#ifndef IPLPROCESS_H
#define IPLPROCESS_H

#include "IPL_global.h"
#include "IPLEvent.h"
#include "IPLData.h"
#include "IPLImage.h"
#include "IPLProcessProperty.h"
#include "IPLProgressEventHandler.h"
#include "IPLPropertyChangedEventHandler.h"
#include "IPLOutputsChangedEventHandler.h"

#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <memory>

#include "pugg/Driver.h"

class IPLProcessProperty;
class IPLProgressEventHandler;
class IPLPropertyChangedEventHandler;
class IPLProcessPropertyString;

//! IPLProcessPropertyMap
typedef std::map<std::string, std::shared_ptr<IPLProcessProperty>> IPLProcessPropertyMap;

/**
 * @brief The IPLProcessMessage struct for passing around messages and errors
 */
struct IPLSHARED_EXPORT IPLProcessMessage
{
    enum IPLProcessMessageType
    {
        SUCCESS,
        INFORMATION,
        WARNING,
        ERR
    };

    IPLProcessMessage(std::string msg, IPLProcessMessageType type)
    {
        this->msg  = msg;
        this->type = type;
    }

    IPLProcessMessageType   type;
    std::string             msg;

};

/**
 * @brief The IPLProcessIO struct
 */
struct IPLSHARED_EXPORT IPLProcessIO
{
    IPLProcessIO()
    {
        this->index = -1;
        this->name = "";
        this->type = IPL_UNDEFINED;
        this->occupied = false;
    }

    IPLProcessIO(int index, std::string name, IPLDataType type)
    {
        this->index = index;
        this->name = name;
        this->type = type;
        this->occupied = false;
    }

    int                     index;
    std::string             name;
    IPLDataType             type;
    bool                    occupied;
};

/**
 * @brief The IPLProcess class
 */
class IPLSHARED_EXPORT IPLProcess
{
public:
    enum IPLProcessCategory
    {
        CATEGORY_UNDEFINED,
        CATEGORY_IO,
        CATEGORY_CONVERSIONS,
        CATEGORY_ARITHMETIC,
        CATEGORY_GEOMETRY,
        CATEGORY_POINTOPERATIONS,
        CATEGORY_LOCALOPERATIONS,
        CATEGORY_MORPHOLOGY,
        CATEGORY_HISTOGRAM,
        CATEGORY_EDGES,
        CATEGORY_GRADIENTS,
        CATEGORY_FOURIER,
        CATEGORY_OBJECTS,
    };

    enum IPLOpenCVSupport
    {
        OPENCV_NONE,
        OPENCV_OPTIONAL,
        OPENCV_ONLY
    };

                            IPLProcess                  (void);
                            IPLProcess                  (const IPLProcess& other);
                            IPLProcess                  (IPLProcess&& other);
    virtual                 ~IPLProcess                 (void);
    virtual IPLProcess*     clone                       () const = 0;
    virtual void            init                        () = 0;
    virtual void            destroy                     () = 0;
    virtual void            beforeProcessing            () {}
    virtual bool            processInputData            (IPLData* data, int inputIndex, bool useOpenCV) = 0;
    virtual void            processPropertyEvents       (IPLEvent* eventName) {};
    virtual IPLData*        getResultData               (int outputIndex ) = 0;
    virtual void            afterProcessing             () {}

    void                    registerProgressEventHandler(IPLProgressEventHandler* handler);
    void                    notifyProgressEventHandler(int percent);
    void                    registerPropertyChangedEventHandler(IPLPropertyChangedEventHandler* handler);
    void                    notifyPropertyChangedEventHandler();
    void                    registerOutputsChangedEventHandler(IPLOutputsChangedEventHandler* handler);
    void                    notifyOutputsChangedEventHandler();

    IPLProcessPropertyMap*  properties();
    IPLProcessProperty*     property(std::string key);
    void                    setProperty(std::string key, IPLProcessProperty* value);
    bool                    isResultReady()                     { return _resultReady; }
    void                    setResultReady(bool ready)          { _resultReady = ready; }
    bool                    updateNeeded()                      { return _updateNeeded; }
    void                    setUpdateNeeded(bool update)        { _updateNeeded = update; }
    void                    requestUpdate();
    void                    requestUpdate(long updateID);

    void                    resetMessages();
    void                    addMessage(IPLProcessMessage msg);
    void                    addError(std::string msg);
    void                    addWarning(std::string msg);
    void                    addSuccess(std::string msg);
    void                    addInformation(std::string msg);
    std::vector<IPLProcessMessage>* messages();
    bool                    hasWarnings();
    bool                    hasErrors();
    bool                    hasMessages();

    std::vector<IPLProcessIO>* inputs                           ()                              { return &_inputs; }
    std::vector<IPLProcessIO>* outputs                          ()                              { return &_outputs; }

    void                    setOutputName                       (int index, std::string name);
    void                    setIsSource                         (bool isSource)                 { _isSource = isSource;  }
    void                    setIsSequence                       (bool isSequence)               { _isSequence = isSequence;  }
    void                    setOpenCVSupport                    (IPLOpenCVSupport supportType)  { _openCVSupport = supportType;  }
    void                    setClassName                        (std::string className)         { _className = className; }
    void                    setTitle                            (std::string title)             { _title = title; }
    void                    setDescription                      (std::string description)       { _description = description; }
    void                    setKeywords                         (std::string keywords)          { _keywords = keywords; }
    void                    setHelpPage                         (std::string helpPage)          { _helpPage = helpPage; }
    void                    setCategory                         (IPLProcessCategory category)   { _category = category; }
    bool                    isSource                            ()                              { return _isSource; }
    bool                    isSequence                          ()                              { return _isSequence; }
    IPLOpenCVSupport        openCVSupport                       ()                              { return _openCVSupport; }
    std::string             title                               ()                              { return _title; }
    std::string             description                         ()                              { return _description; }
    IPLProcessCategory      category                            ()                              { return _category; }
    std::string             className                           ()                              { return _className; }
    std::string             keywords                            ()                              { return _keywords; }
    std::string             helpPage                            ()                              { return _helpPage; }
    int                     availableInputs                     ();
    int                     availableOutputs                    ();

    void                    addInput                            (std::string name, IPLDataType type);
    void                    addOutput                           (std::string name, IPLDataType type);
    void                    addProcessPropertyInt               (const char* name, const char* title, const char* description, int value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT, int min = 0, int max = 0);
    void                    addProcessPropertyUnsignedInt       (const char* name, const char* title, const char* description, unsigned int value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT, unsigned int min = 0, unsigned int max = 0);
    void                    addProcessPropertyDouble            (const char* name, const char* title, const char* description, double value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT, double min = 0.0, double max = 0.0);
    void                    addProcessPropertyFloat             (const char* name, const char* title, const char* description, float value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT, float min = 0.0f, float max = 0.0f);
    void                    addProcessPropertyBool              (const char* name, const char* title, const char* description, bool value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT);
    void                    addProcessPropertyBoolOneShot       (const char* name, const char* title, const char* description, bool value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT);
    void                    addProcessPropertyString            (const char* name, const char* title, const char* description, const std::string &value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT);
    void                    addProcessPropertyVectorInt         (const char* name, const char* title, const char* description, const std::vector<int> &value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT);
    void                    addProcessPropertyVectorDouble      (const char* name, const char* title, const char* description, const std::vector<double> &value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT);
    void                    addProcessPropertyColor             (const char* name, const char* title, const char* description, const IPLColor &value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT);
    void                    addProcessPropertyPoint             (const char* name, const char* title, const char* description, const IPLPoint &value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT);
    int                     getProcessPropertyInt               (const char* name);
    unsigned int            getProcessPropertyUnsignedInt       (const char* name);
    double                  getProcessPropertyDouble            (const char* name);
    float                   getProcessPropertyFloat             (const char* name);
    bool                    getProcessPropertyBool              (const char* name);
    bool                    getProcessPropertyBoolOneShot       (const char* name);
    std::string             getProcessPropertyString            (const char* name);
    std::vector<int>        getProcessPropertyVectorInt         (const char* name);
    std::vector<double>     getProcessPropertyVectorDouble      (const char* name);
    IPLColor                getProcessPropertyColor             (const char* name);
    IPLPoint                getProcessPropertyPoint             (const char* name);

    std::string             toJson();

protected:

private:
    void                    checkPropertyKey                    (const char* name);

    bool                            _isSource;
    bool                            _isSequence;
    bool                            _resultReady;
    bool                            _updateNeeded;
    IPLProgressEventHandler*        _progressHandler;
    IPLPropertyChangedEventHandler* _propertyHandler;
    IPLOutputsChangedEventHandler*  _outputsHandler;
    //std::mutex                    _propertyMutex;
    std::mutex                      _messageMutex;
    std::string                     _className;
    std::string                     _title;
    std::string                     _description;
    std::string                     _keywords;
    std::string                     _helpPage;
    IPLProcessCategory              _category;
    std::vector<IPLProcessIO>       _inputs;
    std::vector<IPLProcessIO>       _outputs;
    IPLProcessPropertyMap           _properties;
    std::vector<IPLProcessMessage>  _messages;
    IPLOpenCVSupport                _openCVSupport;

public:
    // needed for plugins
    static const int version = 1;
    static const std::string server_name() {return "IPLProcessServer";}
};


/*!
 * Derivation helper needed for process cloning
 * http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
 */
template <class Derived>
class IPLClonableProcess : public IPLProcess
{
public:
    virtual IPLProcess* clone() const
    {
        return new Derived(static_cast<const Derived&>(*this)); // call the copy ctor.
    }
};

class IPLProcessDriver : public pugg::Driver
{
public:
    IPLProcessDriver(std::string name, int version) : pugg::Driver(IPLProcess::server_name(),name,version) {}
    virtual IPLProcess* create() = 0;
    virtual std::string className() = 0;
    virtual std::string author() = 0;
    virtual int         version() = 0;

};

#endif // IPLPROCESS_H
