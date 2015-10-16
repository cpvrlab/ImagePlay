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

#include "IPLProcessProperty.h"

#include <array>
#include <cctype>
#include <regex>

template<class T>
inline std::string serializeValue(const T &value)
{
    std::ostringstream buffer;
    buffer << value;
    return buffer.str();
}

template<>
inline std::string serializeValue<bool>(const bool &value)
{
    std::ostringstream buffer;
    buffer <<  value ? "true" : "false";
    return buffer.str();
}

template<>
inline std::string serializeValue<std::vector<int>>(const std::vector<int> &value)
{
    std::ostringstream buffer;
    buffer << "[";
    if (value.size() > 0) buffer << value[0];
    for (int i = 1; i < value.size(); ++i) buffer << "," << value[i];
    buffer << "]";
    return buffer.str();
}

template<>
inline std::string serializeValue<std::vector<double>>(const std::vector<double> &value)
{
    std::ostringstream buffer;
    buffer << "[";
    if (value.size() > 0) buffer << value[0];
    for (int i = 1; i < value.size(); ++i) buffer << "," << value[i];
    buffer << "]";
    return buffer.str();
}

template<>
inline std::string serializeValue<IPLColor>(const IPLColor &value)
{
    std::ostringstream buffer;
    buffer << "[" << value.red() << "," << value.green() << "," << value.blue() << "]";
    return buffer.str();
}

template<>
inline std::string serializeValue<IPLPoint>(const IPLPoint &value)
{
    std::ostringstream buffer;
    buffer << "[" << value.x() << "," << value.y() << "]";
    return buffer.str();
}

template<class T>
inline IPLProcessProperty::SerializedData serializeProperty(const char *type, IPLProcessWidgetType widget, const T &value)
{
    IPLProcessProperty::SerializedData result;
    result.type = serializeValue(type);
    result.widget = serializeValue(widget);
    result.widgetName = serializeValue(widgetName(widget));
    result.value = serializeValue(value);
    return result;
}

inline void deserializeValue(const std::string &data, bool &value)
{
    if (data.length() != 4)
    {
        value = false;
        return;
    }

    static const unsigned char nonAsciiMap = 1<<7;
    std::string lowercase(data);
    //tolower is undefined (i.e. might crash) for non-ASCII characters. Since we don't want to parse those, just xor them out
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), [](char c) { return c ^ nonAsciiMap; });
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower); //Transform to lower case
    value = lowercase.compare("true") == 0;
}

inline void deserializeValue(const std::string &data, int &value)
{
    int result = 0;
    if (sscanf(data.c_str(),"%d",&result) < 1)
        throw IPLProcessProperty::DeserialationFailed();
    value = result;
}

inline void deserializeValue(const std::string &data, IPLProcessWidgetType &value)
{
    int tmp;
    deserializeValue(data,tmp);
    value = (IPLProcessWidgetType) tmp;
}

inline void deserializeValue(const std::string &data, unsigned int &value)
{
    unsigned int result = 0;
    if (sscanf(data.c_str(),"%u",&result) < 1)
        throw IPLProcessProperty::DeserialationFailed();
    value = result;
}

inline void deserializeValue(const std::string &data, float &value)
{
    float result = 0;
    if (sscanf(data.c_str(),"%f",&result) < 1)
        throw IPLProcessProperty::DeserialationFailed();
    value = result;
}

inline void deserializeValue(const std::string &data, double &value)
{
    double result = 0;
    if (sscanf(data.c_str(),"%lf",&result) < 1)
        throw IPLProcessProperty::DeserialationFailed();
    value = result;
}

inline void deserializeValue(const std::string &data, std::vector<int> &value)
{
    std::vector<int> result;
    std::smatch match;
    auto pos = data.begin();
    while(std::regex_search(pos,data.end(),match,std::regex("[0-9]|-")))
    {
        pos += match.position();

        int charsParsed = 0;
        int element = 0;
        if (sscanf(&(*pos),"%d%n",&element,&charsParsed) > 0)
            result.push_back(element);

        pos += charsParsed;
    }
    value.swap(result);
}

inline void deserializeValue(const std::string &data, std::vector<double> &value)
{
    std::vector<double> result;
    std::smatch match;
    auto pos = data.begin();
    while(std::regex_search(pos,data.end(),match,std::regex("[0-9]|-")))
    {
        pos += match.position();

        int charsParsed = 0;
        double element = 0;
        if (sscanf(&(*pos),"%f%n",&element,&charsParsed) > 0)
            result.push_back(element);

        pos += charsParsed;
    }
    value.swap(result);
}

inline void deserializeValue(const std::string &data, IPLColor &value)
{
    std::array<float,3> color;

    int i = 0;
    std::smatch match;
    auto pos = data.begin();
    while(i < color.size() && std::regex_search(pos,data.end(),match,std::regex("[-0-9.]")))
    {
        pos += match.position();

        int charsParsed = 0;
        float element = 0;
        if (sscanf(&(*pos),"%f%n",&element,&charsParsed) > 0)
            color[i++] = element;

        pos += charsParsed;
    }
    value = IPLColor(color[0], color[1], color[2]);
}

inline void deserializeValue(const std::string &data, IPLPoint &value)
{
    std::array<double,2> point;

    int i = 0;
    std::smatch match;
    auto pos = data.begin();
    while(i < point.size() && std::regex_search(pos,data.end(),match,std::regex("[-0-9.]")))
    {
        pos += match.position();

        int charsParsed = 0;
        float element = 0;
        if (sscanf(&(*pos),"%lf%n",&element,&charsParsed) > 0)
            point[i++] = element;

        pos += charsParsed;
    }

    value = IPLPoint(point[0], point[1]);
}

inline void deserializeValue(const std::string &data, std::string &value)
{
    value = data;
}

template<class T>
inline void deserializeProperty(IPLProcessProperty::SerializedData data, IPLProcessWidgetType &widget, T &value)
{
    deserializeValue(data.widget,widget);
    deserializeValue(data.value,value);
}

IPLProcessProperty::IPLProcessProperty(int position, const char* name, const char *title, const char *description, IPLProcess *process, IPLProcessWidgetType widget):
    _position(position),
    _name(name),
    _title(title),
    _description(description),
    _process(process),
    _widget(widget)
{}

IPLProcessPropertyInt::IPLProcessPropertyInt(IPLProcess *process, int position, const char* name, const char* title, const char *description, int value, IPLProcessWidgetType widget, int min, int max):
    IPLProcessProperty(position,name,title,description,process,widget),
    _value(value),
    _default(value),
    _min(min),
    _max(max)
{}

void IPLProcessPropertyInt::setValue(int value)
{
    _value = value;
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

IPLProcessProperty::SerializedData IPLProcessPropertyInt::serialize() const
{
    return serializeProperty(type(),_widget,_value);
}

void IPLProcessPropertyInt::deserialize(const SerializedData &data)
{
    deserializeProperty(data,_widget,_value);
}

IPLProcessProperty *IPLProcessPropertyInt::clone() const
{
    return new IPLProcessPropertyInt(*this);
}

IPLProcessPropertyUnsignedInt::IPLProcessPropertyUnsignedInt(IPLProcess *process, int position, const char* name, const char* title, const char *description, unsigned int value, IPLProcessWidgetType widget, unsigned int min, unsigned int max):
    IPLProcessProperty(position,name,title,description,process,widget),
    _value(value),
    _default(value),
    _min(min),
    _max(max)
{}

void IPLProcessPropertyUnsignedInt::setValue(unsigned int value)
{
    _value = value;
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

IPLProcessProperty::SerializedData IPLProcessPropertyUnsignedInt::serialize() const
{
    return serializeProperty(type(),_widget,_value);
}

void IPLProcessPropertyUnsignedInt::deserialize(const SerializedData &data)
{
    deserializeProperty(data,_widget,_value);
}

IPLProcessProperty *IPLProcessPropertyUnsignedInt::clone() const
{
    return new IPLProcessPropertyUnsignedInt(*this);
}


IPLProcessPropertyDouble::IPLProcessPropertyDouble(IPLProcess *process, int position, const char* name, const char* title, const char *description, double value, IPLProcessWidgetType widget, double min, double max):
    IPLProcessProperty(position,name,title,description,process, widget),
    _value(value),
    _default(value),
    _min(min),
    _max(max)
{}

void IPLProcessPropertyDouble::setValue(double value)
{
    _value = value;
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

IPLProcessProperty::SerializedData IPLProcessPropertyDouble::serialize() const
{
    return serializeProperty(type(),_widget,_value);
}

void IPLProcessPropertyDouble::deserialize(const SerializedData &data)
{
    deserializeProperty(data,_widget,_value);
}

IPLProcessProperty *IPLProcessPropertyDouble::clone() const
{
    return new IPLProcessPropertyDouble(*this);
}


IPLProcessPropertyFloat::IPLProcessPropertyFloat(IPLProcess *process, int position, const char* name, const char* title, const char *description, float value, IPLProcessWidgetType widget, float min, float max):
    IPLProcessProperty(position,name,title,description,process,widget),
    _value(value),
    _default(value),
    _min(min),
    _max(max)
{}

void IPLProcessPropertyFloat::setValue(float value)
{
    _value = value;
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

IPLProcessProperty::SerializedData IPLProcessPropertyFloat::serialize() const
{
    return serializeProperty(type(),_widget,_value);
}

void IPLProcessPropertyFloat::deserialize(const SerializedData &data)
{
    deserializeProperty(data,_widget,_value);
}

IPLProcessProperty *IPLProcessPropertyFloat::clone() const
{
    return new IPLProcessPropertyFloat(*this);
}


IPLProcessPropertyBool::IPLProcessPropertyBool(IPLProcess *process, int position, const char* name, const char* title, const char *description, bool value, IPLProcessWidgetType widget):
    IPLProcessProperty(position,name,title,description,process,widget),
    _value(value),
    _default(value)
{}

void IPLProcessPropertyBool::setValue(bool value)
{
    _value = value;
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

IPLProcessProperty::SerializedData IPLProcessPropertyBool::serialize() const
{
    return serializeProperty(type(),_widget,_value);
}

void IPLProcessPropertyBool::deserialize(const SerializedData &data)
{
    deserializeProperty(data,_widget,_value);
}

IPLProcessProperty *IPLProcessPropertyBool::clone() const
{
    return new IPLProcessPropertyBool(*this);
}


IPLProcessPropertyString::IPLProcessPropertyString(IPLProcess *process, int position, const char* name, const char* title, const char *description, const std::string &value, IPLProcessWidgetType widget):
    IPLProcessProperty(position,name,title,description,process,widget),
    _value(value),
    _default(value)
{}

void IPLProcessPropertyString::setValue(const std::string &value)
{
    _value = value;
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

void IPLProcessPropertyString::setValue(std::string &&value)
{
    _value = std::move(value);
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

IPLProcessProperty::SerializedData IPLProcessPropertyString::serialize() const
{
    return serializeProperty(type(),_widget,_value);
}

void IPLProcessPropertyString::deserialize(const SerializedData &data)
{
    deserializeProperty(data,_widget,_value);
}

IPLProcessProperty *IPLProcessPropertyString::clone() const
{
    return new IPLProcessPropertyString(*this);
}


IPLProcessPropertyVectorInt::IPLProcessPropertyVectorInt(IPLProcess *process, int position, const char* name, const char* title, const char *description, const std::vector<int> &value, IPLProcessWidgetType widget):
    IPLProcessProperty(position,name,title,description,process,widget),
    _value(value)
{
    _default = value;
}

void IPLProcessPropertyVectorInt::setValue(const std::vector<int> &value)
{
    _value = value;
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

void IPLProcessPropertyVectorInt::setValue(std::vector<int> &&value)
{
    _value = std::move(value);
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

IPLProcessProperty::SerializedData IPLProcessPropertyVectorInt::serialize() const
{
    return serializeProperty(type(),_widget,_value);
}

void IPLProcessPropertyVectorInt::deserialize(const SerializedData &data)
{
    deserializeProperty(data,_widget,_value);
}

IPLProcessProperty *IPLProcessPropertyVectorInt::clone() const
{
    return new IPLProcessPropertyVectorInt(*this);
}



IPLProcessPropertyVectorDouble::IPLProcessPropertyVectorDouble(IPLProcess *process, int position, const char* name, const char* title, const char *description, const std::vector<double> &value, IPLProcessWidgetType widget):
    IPLProcessProperty(position,name,title,description,process,widget),
    _value(value),
    _default(value)
{}

void IPLProcessPropertyVectorDouble::setValue(const std::vector<double> &value)
{
    _value = value;
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

void IPLProcessPropertyVectorDouble::setValue(std::vector<double> &&value)
{
    _value = std::move(value);
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

IPLProcessProperty::SerializedData IPLProcessPropertyVectorDouble::serialize() const
{
    return serializeProperty(type(),_widget,_value);
}

void IPLProcessPropertyVectorDouble::deserialize(const SerializedData &data)
{
    deserializeProperty(data,_widget,_value);
}

IPLProcessProperty *IPLProcessPropertyVectorDouble::clone() const
{
    return new IPLProcessPropertyVectorDouble(*this);
}


IPLProcessPropertyColor::IPLProcessPropertyColor(IPLProcess *process, int position, const char* name, const char* title, const char *description, const IPLColor &value, IPLProcessWidgetType widget):
    IPLProcessProperty(position,name,title,description,process,widget),
    _value(value),
    _default(value)
{}

void IPLProcessPropertyColor::setValue(const IPLColor &value)
{
    _value = value;
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

void IPLProcessPropertyColor::setValue(IPLColor &&value)
{
    _value = std::move(value);
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

IPLProcessProperty::SerializedData IPLProcessPropertyColor::serialize() const
{
    return serializeProperty(type(),_widget,_value);
}

void IPLProcessPropertyColor::deserialize(const SerializedData &data)
{
    deserializeProperty(data,_widget,_value);
}

IPLProcessProperty *IPLProcessPropertyColor::clone() const
{
    return new IPLProcessPropertyColor(*this);
}

IPLProcessPropertyPoint::IPLProcessPropertyPoint(IPLProcess *process, int position, const char* name, const char* title, const char *description, const IPLPoint &value, IPLProcessWidgetType widget):
    IPLProcessProperty(position,name,title,description,process,widget),
    _value(value),
    _default(value)
{}

void IPLProcessPropertyPoint::setValue(const IPLPoint &value)
{
    _value = value;
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

void IPLProcessPropertyPoint::setValue(IPLPoint &&value)
{
    _value = std::move(value);
    _process->requestUpdate();
    _process->notifyPropertyChangedEventHandler();
}

IPLProcessProperty::SerializedData IPLProcessPropertyPoint::serialize() const
{
    return serializeProperty(type(),_widget,_value);
}

void IPLProcessPropertyPoint::deserialize(const SerializedData &data)
{
    deserializeProperty(data,_widget,_value);
}

IPLProcessProperty *IPLProcessPropertyPoint::clone() const
{
    return new IPLProcessPropertyPoint(*this);
}
