#include "IPLProcessProperty.h"


IPLProcessProperty::IPLProcessProperty(int position, const char *name, const char *description, IPLProcess *process, IPLProcessWidgetType widget):
    _position(position),
    _name(name),
    _description(description),
    _process(process),
    _widget(widget)
{}

IPLProcessPropertyInt::IPLProcessPropertyInt(IPLProcess *process, int position, const char *name, const char *description, int value, IPLProcessWidgetType widget, int min, int max):
    IPLProcessProperty(position,name,description,process,widget),
    _value(value),
    _min(min),
    _max(max)
{}

void IPLProcessPropertyInt::setValue(int value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyInt::toJson() const
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"int\",\n";
    json << "   \"type\": \"" << _widget << "\", //" << widgetName(_widget) << "\n";
    json << "   \"value\": \""  << _value << "\"\n";
    json << "}";
    return json.str();
}

IPLProcessProperty *IPLProcessPropertyInt::clone() const
{
    return new IPLProcessPropertyInt(*this);
}

void IPLProcessPropertyInt::fromJson(std::string value)
{
    _value = atoi(value.c_str());
}


IPLProcessPropertyUnsignedInt::IPLProcessPropertyUnsignedInt(IPLProcess *process, int position, const char *name, const char *description, unsigned int value, IPLProcessWidgetType widget, unsigned int min, unsigned int max):
    IPLProcessProperty(position,name,description,process,widget),
    _value(value),
    _min(min),
    _max(max)
{}

void IPLProcessPropertyUnsignedInt::setValue(unsigned int value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyUnsignedInt::toJson() const
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"uint\",\n";
    json << "   \"type\": \"" << _widget << "\", //" << widgetName(_widget) << "\n";
    json << "   \"value\": \""  << _value << "\"\n";
    json << "}";
    return json.str();
}

void IPLProcessPropertyUnsignedInt::fromJson(std::string value)
{
    _value = (unsigned int) atol(value.c_str());
}

IPLProcessProperty *IPLProcessPropertyUnsignedInt::clone() const
{
    return new IPLProcessPropertyUnsignedInt(*this);
}


IPLProcessPropertyDouble::IPLProcessPropertyDouble(IPLProcess *process, int position, const char *name, const char *description, double value, IPLProcessWidgetType widget, double min, double max):
    IPLProcessProperty(position,name,description,process, widget),
    _value(value),
    _min(min),
    _max(max)
{}

void IPLProcessPropertyDouble::setValue(double value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyDouble::toJson() const
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"double\",\n";
    json << "   \"type\": \"" << _widget << "\", //" << widgetName(_widget) << "\n";
    json << "   \"value\": \""  << _value << "\"\n";
    json << "}";
    return json.str();
}

void IPLProcessPropertyDouble::fromJson(std::string value)
{
    _value = atof(value.c_str());
}

IPLProcessProperty *IPLProcessPropertyDouble::clone() const
{
    return new IPLProcessPropertyDouble(*this);
}


IPLProcessPropertyFloat::IPLProcessPropertyFloat(IPLProcess *process, int position, const char *name, const char *description, float value, IPLProcessWidgetType widget, float min, float max):
    IPLProcessProperty(position,name,description,process,widget),
    _value(value),
    _min(min),
    _max(max)
{}

void IPLProcessPropertyFloat::setValue(float value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyFloat::toJson() const
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"float\",\n";
    json << "   \"type\": \"" << _widget << "\", //" << widgetName(_widget) << "\n";
    json << "   \"value\": \""  << _value << "\"\n";
    json << "}";
    return json.str();
}

void IPLProcessPropertyFloat::fromJson(std::string value)
{
    _value = atof(value.c_str());
}

IPLProcessProperty *IPLProcessPropertyFloat::clone() const
{
    return new IPLProcessPropertyFloat(*this);
}


IPLProcessPropertyBool::IPLProcessPropertyBool(IPLProcess *process, int position, const char *name, const char *description, bool value, IPLProcessWidgetType widget):
    IPLProcessProperty(position,name,description,process,widget),
    _value(value)
{}

void IPLProcessPropertyBool::setValue(bool value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyBool::toJson() const
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"bool\",\n";
    json << "   \"type\": \"" << _widget << "\", //" << widgetName(_widget) << "\n";
    if(_value)
        json << "   \"value\": true\n";
    else
        json << "   \"value\": false\n";
    json << "}";
    return json.str();
}

void IPLProcessPropertyBool::fromJson(std::string value)
{
    if(value == "true")
        _value = true;
    else
        _value = false;
}

IPLProcessProperty *IPLProcessPropertyBool::clone() const
{
    return new IPLProcessPropertyBool(*this);
}


IPLProcessPropertyString::IPLProcessPropertyString(IPLProcess *process, int position, const char *name, const char *description, const std::string &value, IPLProcessWidgetType widget):
    IPLProcessProperty(position,name,description,process,widget),
    _value(value)
{}

void IPLProcessPropertyString::setValue(const std::string &value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

void IPLProcessPropertyString::setValue(std::string &&value)
{
    _value = std::move(value);
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyString::toJson() const
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"string\",\n";
    json << "   \"type\": \"" << _widget << "\", //" << widgetName(_widget) << "\n";
    json << "   \"value\": \"" << _value << "\"\n";
    json << "}";
    return json.str();
}

void IPLProcessPropertyString::fromJson(std::string value)
{
    _value = value;
}

IPLProcessProperty *IPLProcessPropertyString::clone() const
{
    return new IPLProcessPropertyString(*this);
}


IPLProcessPropertyVectorInt::IPLProcessPropertyVectorInt(IPLProcess *process, int position, const char *name, const char *description, const std::vector<int> &value, IPLProcessWidgetType widget):
    IPLProcessProperty(position,name,description,process,widget),
    _value(value)
{}

void IPLProcessPropertyVectorInt::setValue(const std::vector<int> &value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

void IPLProcessPropertyVectorInt::setValue(std::vector<int> &&value)
{
    _value = std::move(value);
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyVectorInt::toJson() const
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"vector<int>\",\n";
    json << "   \"type\": \"" << _widget << "\", //" << widgetName(_widget) << "\n";
    json << "   \"value\": \"";
    for(size_t i=0; i<_value.size(); i++)
    {
        json << _value[i];
        if(i<_value.size()-1)
            json << "|";
    }
    json << "\"\n";
    json << "}";
    return json.str();
}

void IPLProcessPropertyVectorInt::fromJson(std::string value)
{
    _value.clear();
    char* input = (char*) value.c_str();
    char* token = strtok(input, "|");
    while(token != NULL)
    {
        _value.push_back(atoi(token));
        token = strtok(NULL, "|");
    }
}

IPLProcessProperty *IPLProcessPropertyVectorInt::clone() const
{
    return new IPLProcessPropertyVectorInt(*this);
}


IPLProcessPropertyColor::IPLProcessPropertyColor(IPLProcess *process, int position, const char *name, const char *description, const IPLColor &value, IPLProcessWidgetType widget):
    IPLProcessProperty(position,name,description,process,widget),
    _value(value)
{}

void IPLProcessPropertyColor::setValue(const IPLColor &value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

void IPLProcessPropertyColor::setValue(IPLColor &&value)
{
    _value = std::move(value);
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyColor::toJson() const
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"color\",\n";
    json << "   \"type\": \"" << _widget << "\", //" << widgetName(_widget) << "\n";
    json << "   \"value\": \""  << _value.red() << "|" << _value.green() << "|"<< _value.blue() << "|" << "\"\n";
    json << "}";
    return json.str();
}

void IPLProcessPropertyColor::fromJson(std::string value)
{
    char* input = (char*) value.c_str();
    char* token1 = strtok(input, "|");
    char* token2 = strtok(token1, "|");
    char* token3 = strtok(token2, "|");
    _value = IPLColor::fromRGB(atof(token1), atof(token2), atof(token3));
}

IPLProcessProperty *IPLProcessPropertyColor::clone() const
{
    return new IPLProcessPropertyColor(*this);
}

IPLProcessPropertyPoint::IPLProcessPropertyPoint(IPLProcess *process, int position, const char *name, const char *description, const IPLPoint &value, IPLProcessWidgetType widget):
    IPLProcessProperty(position,name,description,process,widget),
    _value(value)
{}

void IPLProcessPropertyPoint::setValue(const IPLPoint &value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

void IPLProcessPropertyPoint::setValue(IPLPoint &&value)
{
    _value = std::move(value);
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyPoint::toJson() const
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"point\",\n";
    json << "   \"type\": \"" << _widget << "\", //" << widgetName(_widget) << "\n";
    json << "   \"value\": \""  << _value.x() << "|" << _value.y() << "\"\n";
    json << "}";
    return json.str();
}

void IPLProcessPropertyPoint::fromJson(std::string value)
{
    char* input = (char*) value.c_str();
    char* token1 = strtok(input, "|");
    char* token2 = strtok(token1, "|");
    _value = IPLPoint(atof(token1), atof(token2));
}

IPLProcessProperty *IPLProcessPropertyPoint::clone() const
{
    return new IPLProcessPropertyPoint(*this);
}
