#ifndef IPLPROCESS_H
#define IPLPROCESS_H

#include "IPL_global.h"
#include "IPLData.h"
#include "IPLImage.h"
#include "IPLProcessProperty.h"
#include "IPLProgressEventHandler.h"
#include "IPLPropertyChangedEventHandler.h"

#include <string>
#include <map>
#include <vector>
#include <mutex>

class IPLProcessProperty;
class IPLProgressEventHandler;
class IPLPropertyChangedEventHandler;

//! IPLProcessPropertyMap
typedef std::map<std::string, IPLProcessProperty*> IPLProcessPropertyMap;

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
        this->type = IPLData::UNDEFINED;
        this->occupied = false;
    }

    IPLProcessIO(int index, std::string name, IPLData::IPLDataType type)
    {
        this->index = index;
        this->name = name;
        this->type = type;
        this->occupied = false;
    }

    int                     index;
    std::string             name;
    IPLData::IPLDataType    type;
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
    virtual                 ~IPLProcess                 (void);
    virtual IPLProcess*     clone                       () const = 0;
    virtual void            init                        () = 0;
    virtual void            destroy                     () = 0;
    virtual bool            processInputData            (IPLImage* data, int inputIndex, bool useOpenCV) = 0;
    virtual IPLData*        getResultData               (int outputIndex ) = 0;

    void                    registerProgressEventHandler(IPLProgressEventHandler* handler);
    void                    notifyProgressEventHandler(int percent);
    void                    registerPropertyChangedEventHandler(IPLPropertyChangedEventHandler* handler);
    void                    notifyPropertyChangedEventHandler();

    IPLProcessPropertyMap*  properties();
    IPLProcessProperty*     property(std::string key);
    void                    setProperty(std::string key, IPLProcessProperty* value);
    bool                    isResultReady()                    { return _resultReady; }
    void                    setResultReady(bool ready)         { _resultReady = ready; }
    bool                    needsUpdate()                      { return _needsUpdate; }
    void                    setNeedsUpdate(bool needsUpdate)   { _needsUpdate = needsUpdate; }

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

    std::vector<IPLProcessIO>* inputs                       ()     { return &_inputs; }
    std::vector<IPLProcessIO>* outputs                      ()    { return &_outputs; }

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

    void                    addInput                            (std::string name, IPLData::IPLDataType type);
    void                    addOutput                           (std::string name, IPLData::IPLDataType type);
    void                    addProcessPropertyInt               (const char* name, const char* title, const char* description, IPLProcessPropertyType type, int value, int min = 0, int max = 0);
    void                    addProcessPropertyUnsignedInt       (const char* name, const char* title, const char* description, IPLProcessPropertyType type, unsigned int value, unsigned int min = 0, unsigned int max = 0);
    void                    addProcessPropertyDouble            (const char* name, const char* title, const char* description, IPLProcessPropertyType type, double value, double min = 0.0, double max = 0.0);
    void                    addProcessPropertyFloat             (const char* name, const char* title, const char* description, IPLProcessPropertyType type, float value, float min = 0.0f, float max = 0.0f);
    void                    addProcessPropertyBool              (const char* name, const char* title, const char* description, IPLProcessPropertyType type, bool value);
    void                    addProcessPropertyString            (const char* name, const char* title, const char* description, IPLProcessPropertyType type, std::string value);
    void                    addProcessPropertyVectorInt         (const char* name, const char* title, const char* description, IPLProcessPropertyType type, std::vector<int> value);
    void                    addProcessPropertyColor             (const char* name, const char* title, const char* description, IPLProcessPropertyType type, IPLColor value);
    void                    addProcessPropertyPoint             (const char* name, const char* title, const char* description, IPLProcessPropertyType type, IPLPoint value);
    int                     getProcessPropertyInt               (const char* name);
    unsigned int            getProcessPropertyUnsignedInt       (const char* name);
    double                  getProcessPropertyDouble            (const char* name);
    float                   getProcessPropertyFloat             (const char* name);
    bool                    getProcessPropertyBool              (const char* name);
    std::string             getProcessPropertyString            (const char* name);
    std::vector<int>        getProcessPropertyVectorInt         (const char* name);
    IPLColor                getProcessPropertyColor             (const char* name);
    IPLPoint                getProcessPropertyPoint             (const char* name);

    std::string             toJson();

protected:

private:
    void                    checkPropertyKey                    (const char* name);

    bool                            _isSource;
    bool                            _isSequence;
    bool                            _resultReady;
    bool                            _needsUpdate;
    IPLProgressEventHandler*        _handler;
    IPLPropertyChangedEventHandler* _propertyHandler;
    //std::mutex                    _propertyMutex;
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

#endif // IPLPROCESS_H
