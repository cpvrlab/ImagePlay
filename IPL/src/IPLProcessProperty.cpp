#include "IPLProcessProperty.h"


IPLProcessPropertyInt::IPLProcessPropertyInt(IPLProcess *process, int position, const char *name, const char *description, IPLProcessPropertyType type, int value, int min, int max)
{
    _process = process;
    _position = position;
    _name = name;
    _description = description;
    _type = type;
    _value = value;
    _min = min;
    _max = max;
}

void IPLProcessPropertyInt::setValue(int value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyInt::toJson()
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"int\",\n";
    json << "   \"value\": \""  << _value << "\"\n";
    json << "}";
    return json.str();
}

IPLProcessProperty *IPLProcessPropertyInt::clone()
{
    return new IPLProcessPropertyInt(*this);
}

void IPLProcessPropertyInt::fromJson(std::string value)
{
    _value = atoi(value.c_str());
}


IPLProcessPropertyUnsignedInt::IPLProcessPropertyUnsignedInt(IPLProcess *process, int position, const char *name, const char *description, IPLProcessPropertyType type, unsigned int value, unsigned int min, unsigned int max)
{
    _process = process;
    _position = position;
    _name = name;
    _description = description;
    _type = type;
    _value = value;
    _min = min;
    _max = max;
}

void IPLProcessPropertyUnsignedInt::setValue(unsigned int value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyUnsignedInt::toJson()
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"uint\",\n";
    json << "   \"value\": \""  << _value << "\"\n";
    json << "}";
    return json.str();
}

void IPLProcessPropertyUnsignedInt::fromJson(std::string value)
{
    _value = (unsigned int) atol(value.c_str());
}

IPLProcessProperty *IPLProcessPropertyUnsignedInt::clone()
{
    return new IPLProcessPropertyUnsignedInt(*this);
}


IPLProcessPropertyDouble::IPLProcessPropertyDouble(IPLProcess *process, int position, const char *name, const char *description, IPLProcessPropertyType type, double value, double min, double max)
{
    _process = process;
    _position = position;
    _name = name;
    _description = description;
    _type = type;
    _value = value;
    _min = min;
    _max = max;
}

void IPLProcessPropertyDouble::setValue(double value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyDouble::toJson()
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"double\",\n";
    json << "   \"value\": \""  << _value << "\"\n";
    json << "}";
    return json.str();
}

void IPLProcessPropertyDouble::fromJson(std::string value)
{
    _value = atof(value.c_str());
}

IPLProcessProperty *IPLProcessPropertyDouble::clone()
{
    return new IPLProcessPropertyDouble(*this);
}


IPLProcessPropertyFloat::IPLProcessPropertyFloat(IPLProcess *process, int position, const char *name, const char *description, IPLProcessPropertyType type, float value, float min, float max)
{
    _process = process;
    _position = position;
    _name = name;
    _description = description;
    _type = type;
    _value = value;
    _min = min;
    _max = max;
}

void IPLProcessPropertyFloat::setValue(float value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyFloat::toJson()
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"float\",\n";
    json << "   \"value\": \""  << _value << "\"\n";
    json << "}";
    return json.str();
}

void IPLProcessPropertyFloat::fromJson(std::string value)
{
    _value = atof(value.c_str());
}

IPLProcessProperty *IPLProcessPropertyFloat::clone()
{
    return new IPLProcessPropertyFloat(*this);
}


IPLProcessPropertyBool::IPLProcessPropertyBool(IPLProcess *process, int position, const char *name, const char *description, IPLProcessPropertyType type, bool value)
{
    _process = process;
    _position = position;
    _name = name;
    _description = description;
    _type = type;
    _value = value;
    _min = false;
    _max = true;
}

void IPLProcessPropertyBool::setValue(bool value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyBool::toJson()
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"bool\",\n";
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

IPLProcessProperty *IPLProcessPropertyBool::clone()
{
    return new IPLProcessPropertyBool(*this);
}


IPLProcessPropertyString::IPLProcessPropertyString(IPLProcess *process, int position, const char *name, const char *description, IPLProcessPropertyType type, std::string value)
{
    _process = process;
    _position = position;
    _name = name;
    _description = description;
    _type = type;
    _value = value;
    _min = "";
    _max = "";
}

void IPLProcessPropertyString::setValue(std::string value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyString::toJson()
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"string\",\n";
    json << "   \"value\": \"" << _value << "\"\n";
    json << "}";
    return json.str();
}

void IPLProcessPropertyString::fromJson(std::string value)
{
    _value = value;
}

IPLProcessProperty *IPLProcessPropertyString::clone()
{
    return new IPLProcessPropertyString(*this);
}


IPLProcessPropertyVectorInt::IPLProcessPropertyVectorInt(IPLProcess *process, int position, const char *name, const char *description, IPLProcessPropertyType type, std::vector<int> value)
{
    _process = process;
    _position = position;
    _name = name;
    _description = description;
    _type = type;
    _value = value;
}

void IPLProcessPropertyVectorInt::setValue(std::vector<int> value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyVectorInt::toJson()
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"vector<int>\",\n";
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

IPLProcessProperty *IPLProcessPropertyVectorInt::clone()
{
    return new IPLProcessPropertyVectorInt(*this);
}


IPLProcessPropertyColor::IPLProcessPropertyColor(IPLProcess *process, int position, const char *name, const char *description, IPLProcessPropertyType type, IPLColor value)
{
    _process = process;
    _position = position;
    _name = name;
    _description = description;
    _type = type;
    _value = value;
}

void IPLProcessPropertyColor::setValue(IPLColor value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyColor::toJson()
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"color\",\n";
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

IPLProcessProperty *IPLProcessPropertyColor::clone()
{
    return new IPLProcessPropertyColor(*this);
}

IPLProcessPropertyPoint::IPLProcessPropertyPoint(IPLProcess *process, int position, const char *name, const char *description, IPLProcessPropertyType type, IPLPoint value)
{
    _process = process;
    _position = position;
    _name = name;
    _description = description;
    _type = type;
    _value = value;
}

void IPLProcessPropertyPoint::setValue(IPLPoint value)
{
    _value = value;
    _process->setNeedsUpdate(true);
    _process->notifyPropertyChangedEventHandler();
}

std::string IPLProcessPropertyPoint::toJson()
{
    std::ostringstream json;
    json << "{\n";
    json << "   \"type\": \"point\",\n";
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

IPLProcessProperty *IPLProcessPropertyPoint::clone()
{
    return new IPLProcessPropertyPoint(*this);
}
