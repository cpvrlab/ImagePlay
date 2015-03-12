#include "IPLProcess.h"

IPLProcess::IPLProcess(void)
{
    _isSource           = false;
    _isSequence         = false;
    _resultReady        = false;
    _needsUpdate        = true;
    _openCVSupport      = IPLProcess::OPENCV_NONE;
    _handler            = NULL;
    _propertyHandler    = NULL;
    _category           = IPLProcess::CATEGORY_UNDEFINED;

    _properties["title"] = new IPLProcessPropertyString(this, -1, "Title", "", IPL_STRING, _title);
}

//! copy constructor
IPLProcess::IPLProcess(const IPLProcess &other)
{
    _className          = other._className;
    _handler            = other._handler;
    _propertyHandler    = other._propertyHandler;
    _isSource           = other._isSource;
    _isSequence         = other._isSequence;
    _resultReady        = other._resultReady;
    _title              = other._title;
    _properties         = other._properties;
    _category           = other._category;
    _keywords           = other._keywords;
    _openCVSupport      = other._openCVSupport;

    _properties["title"] = new IPLProcessPropertyString(this, -1, "Title", "", IPL_STRING, _title);
}

IPLProcess::~IPLProcess( void )
{

}

int IPLProcess::availableInputs()
{
    int count = 0;
    for(size_t i = 0; i < _inputs.size(); i++)
    {
        if(_inputs.at(i).occupied == false)
        {
            count++;
        }
    }
    return count;
}

int IPLProcess::availableOutputs()
{
    int count = 0;
    for(size_t i = 0; i < _outputs.size(); i++)
    {
        if(_outputs.at(i).occupied == false)
        {
            count++;
        }
    }
    return count;
}

void IPLProcess::addInput(std::string name, IPLData::IPLDataType type)
{
    // automatically increment ID
    _inputs.push_back(IPLProcessIO(_inputs.size(), name, type));
}

void IPLProcess::addOutput(std::string name, IPLData::IPLDataType type)
{
    // automatically increment ID
    _outputs.push_back(IPLProcessIO(_outputs.size(), name, type));
}

void IPLProcess::addProcessPropertyInt(const char *name, const char *title, const char *description, IPLProcessPropertyType type, int value, int min, int max)
{
    _properties[name] = new IPLProcessPropertyInt(this, _properties.size(), title, description, type, value, min, max);
}

void IPLProcess::addProcessPropertyUnsignedInt(const char *name, const char *title, const char *description, IPLProcessPropertyType type, unsigned int value, unsigned int min, unsigned int max)
{
    _properties[name] = new IPLProcessPropertyUnsignedInt(this, _properties.size(), title, description, type, value, min, max);
}

void IPLProcess::addProcessPropertyDouble(const char *name, const char *title, const char *description, IPLProcessPropertyType type, double value, double min, double max)
{
    _properties[name] = new IPLProcessPropertyDouble(this, _properties.size(), title, description, type, value, min, max);
}

void IPLProcess::addProcessPropertyFloat(const char *name, const char *title, const char *description, IPLProcessPropertyType type, float value, float min, float max)
{
    _properties[name] = new IPLProcessPropertyFloat(this, _properties.size(), title, description, type, value, min, max);
}

void IPLProcess::addProcessPropertyBool(const char *name, const char *title, const char *description, IPLProcessPropertyType type, bool value)
{
    _properties[name] = new IPLProcessPropertyBool(this, _properties.size(), title, description, type, value);
}

void IPLProcess::addProcessPropertyString(const char *name, const char *title, const char *description, IPLProcessPropertyType type, std::string value)
{
    _properties[name] = new IPLProcessPropertyString(this, _properties.size(), title, description, type, value);
}

void IPLProcess::addProcessPropertyVectorInt(const char *name, const char *title, const char *description, IPLProcessPropertyType type, std::vector<int> value)
{
    _properties[name] = new IPLProcessPropertyVectorInt(this, _properties.size(), title, description, type, value);
}

void IPLProcess::addProcessPropertyColor(const char *name, const char *title, const char *description, IPLProcessPropertyType type, IPLColor value)
{
    _properties[name] = new IPLProcessPropertyColor(this, _properties.size(), title, description, type, value);
}

void IPLProcess::addProcessPropertyPoint(const char *name, const char *title, const char *description, IPLProcessPropertyType type, IPLPoint value)
{
    _properties[name] = new IPLProcessPropertyPoint(this, _properties.size(), title, description, type, value);
}

int IPLProcess::getProcessPropertyInt(const char *name)
{
    checkPropertyKey(name);
    return ((IPLProcessPropertyInt*) _properties[name])->value();
}

unsigned int IPLProcess::getProcessPropertyUnsignedInt(const char *name)
{
    checkPropertyKey(name);
    return ((IPLProcessPropertyUnsignedInt*) _properties[name])->value();
}

double IPLProcess::getProcessPropertyDouble(const char *name)
{
    checkPropertyKey(name);
    return ((IPLProcessPropertyDouble*) _properties[name])->value();
}

float IPLProcess::getProcessPropertyFloat(const char *name)
{
    checkPropertyKey(name);
    return ((IPLProcessPropertyFloat*) _properties[name])->value();
}

bool IPLProcess::getProcessPropertyBool(const char *name)
{
    checkPropertyKey(name);
    return ((IPLProcessPropertyBool*) _properties[name])->value();
}

std::string IPLProcess::getProcessPropertyString(const char *name)
{
    checkPropertyKey(name);
    return ((IPLProcessPropertyString*) _properties[name])->value();
}

std::vector<int> IPLProcess::getProcessPropertyVectorInt(const char *name)
{
    checkPropertyKey(name);
    return ((IPLProcessPropertyVectorInt*) _properties[name])->value();
}

IPLColor IPLProcess::getProcessPropertyColor(const char *name)
{
    checkPropertyKey(name);
    return ((IPLProcessPropertyColor*) _properties[name])->value();
}

IPLPoint IPLProcess::getProcessPropertyPoint(const char *name)
{
    checkPropertyKey(name);
    return ((IPLProcessPropertyPoint*) _properties[name])->value();
}

//template<class T> void IPLProcess::addProcessProperty(const char *name, const char *title, const char *description, int type, T value, T min /* =0 */, T max /* = 0 */)

void IPLProcess::setOutputName(int index, std::string name)
{
    _outputs.at(index).name = name;
}

std::string IPLProcess::toJson()
{
    std::ostringstream json;
    for(auto it = _properties.begin(); it != _properties.end(); ++it)
    {
        IPLProcessProperty* property = (IPLProcessProperty*) it->second;
        json << "{\n";
        json << "   \"type\": \"bool\"\n";
        json << "   \"value\": \"" << property->type() << "\"\n";
        json << "}\n";
    }
    return json.str();
}

void IPLProcess::checkPropertyKey(const char *name)
{
    if (_properties.find(name) == _properties.end())
    {
        std::string error("Invalid property key: ");
        error.append(name);
        throw std::runtime_error(error.c_str());
    }
}



void IPLProcess::registerProgressEventHandler(IPLProgressEventHandler *handler)
{
    _handler = handler;
}

void IPLProcess::notifyProgressEventHandler(int percent)
{
    if(_handler != NULL)
    {
        _handler->updateProgress(percent);
    }
}

void IPLProcess::registerPropertyChangedEventHandler(IPLPropertyChangedEventHandler *handler)
{
    _propertyHandler = handler;
}

void IPLProcess::notifyPropertyChangedEventHandler()
{
    if(_propertyHandler != NULL)
    {
        _propertyHandler->propertyChanged(this);
    }
}

IPLProcessPropertyMap *IPLProcess::properties()
{
    return &_properties;
}

IPLProcessProperty *IPLProcess::property(std::string key)
{
    if(_properties.count(key))
        return _properties[key];
    else
        return NULL;
}

void IPLProcess::setProperty(std::string key, IPLProcessProperty *value)
{
    //_propertyMutex.lock();
    _properties[key] = value;
    //_propertyMutex.unlock();
}

void IPLProcess::resetMessages()
{
    _messages.clear();
}

void IPLProcess::addMessage(IPLProcessMessage msg)
{
    _messages.push_back(msg);
}

void IPLProcess::addError(std::string msg)
{
    addMessage(IPLProcessMessage(msg, IPLProcessMessage::ERR));
}

void IPLProcess::addWarning(std::string msg)
{
    addMessage(IPLProcessMessage(msg, IPLProcessMessage::WARNING));
}

void IPLProcess::addSuccess(std::string msg)
{
    addMessage(IPLProcessMessage(msg, IPLProcessMessage::SUCCESS));
}

void IPLProcess::addInformation(std::string msg)
{
    addMessage(IPLProcessMessage(msg, IPLProcessMessage::INFORMATION));
}

std::vector<IPLProcessMessage> *IPLProcess::messages()
{
    return &_messages;
}


bool IPLProcess::hasMessages()
{
    return _messages.size() > 0;
}

bool IPLProcess::hasErrors()
{
    for(IPLProcessMessage msg : _messages)
    {
        if(msg.type == IPLProcessMessage::ERR)
            return true;
    }
    return false;
}

bool IPLProcess::hasWarnings()
{
    for(IPLProcessMessage msg : _messages)
    {
        if(msg.type == IPLProcessMessage::WARNING)
            return true;
    }
    return false;
}
