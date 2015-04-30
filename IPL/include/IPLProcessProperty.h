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

#ifndef IPLPROCESSPROPERTY_H
#define IPLPROCESSPROPERTY_H

#include "IPLProcess.h"
#include "IPLColor.h"
#include "IPLPoint.h"

#include <cstring>
#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <memory>

class IPLProcess;


// we need a base class in order to use the template in std::map
class IPLSHARED_EXPORT IPLProcessProperty
{
public:
    struct SerializedData
    {
        std::string type;
        std::string widget;
        std::string widgetName;
        std::string value;
    };

    struct DeserialationFailed : public std::runtime_error
    { DeserialationFailed(): std::runtime_error("") {} };

    int position() const                  { return _position; }
    const char* name() const              { return _name; }
    const char* description() const       { return _description; }
    IPLProcessWidgetType widget() const   { return _widget; }
    virtual const char* type() const = 0;
    virtual SerializedData serialize() const = 0;
    virtual void deserialize(const SerializedData &data) = 0;
    virtual IPLProcessProperty *clone() const = 0;

protected:
    IPLProcessProperty(int position,
                       const char *name,
                       const char *description,
                       IPLProcess *process,
                       IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT);

    int _position;                  //!< Position in GUI
    const char* _name;              //!< Short name for GUI
    const char* _description;       //!< Short help for GUI
    IPLProcess*  _process;
    IPLProcessWidgetType _widget;
};


// templates are not possible because of serialization issues

// INT
class IPLSHARED_EXPORT IPLProcessPropertyInt : public IPLProcessProperty
{
public:
    IPLProcessPropertyInt(IPLProcess* process, int position, const char* name, const char* description, int value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT, int min=0, int max=0);

    int min() const                         { return _min; }
    int max() const                         { return _max; }
    int value() const                       { return _value; }
    void setValue(int value);
    virtual const char *type() const        { return "int"; }
    virtual SerializedData serialize() const;
    virtual void deserialize(const SerializedData &data);
    IPLProcessProperty *clone() const;

private:
    int _min;                         //!< min value, may be used in GUI
    int _max;                         //!< max value, may be used in GUI
    int _value;                       //!< default and also current value
};

// UNSIGNED INT
class IPLSHARED_EXPORT IPLProcessPropertyUnsignedInt: public IPLProcessProperty
{
public:
    IPLProcessPropertyUnsignedInt(IPLProcess* process, int position, const char* name, const char* description, unsigned int value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT, unsigned int min=0, unsigned int max=0);

    unsigned int min() const                         { return _min; }
    unsigned int max() const                         { return _max; }
    unsigned int value() const                       { return _value; }
    void setValue(unsigned int value);
    virtual const char *type() const                 { return "uint"; }
    virtual SerializedData serialize() const;
    virtual void deserialize(const SerializedData &data);
    IPLProcessProperty *clone() const;

private:
    unsigned int _min;                         //!< min value, may be used in GUI
    unsigned int _max;                         //!< max value, may be used in GUI
    unsigned int _value;                       //!< default and also current value
};

// DOUBLE
class IPLSHARED_EXPORT IPLProcessPropertyDouble : public IPLProcessProperty
{
public:
    IPLProcessPropertyDouble(IPLProcess* process, int position, const char* name, const char* description, double value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT, double min=0.0, double max=0.0);

    double min() const                         { return _min; }
    double max() const                         { return _max; }
    double value() const                       { return _value; }
    void setValue(double value);
    virtual const char *type() const           { return "double"; }
    virtual SerializedData serialize() const;
    virtual void deserialize(const SerializedData &data);
    IPLProcessProperty *clone() const;

private:
    double _min;                         //!< min value, may be used in GUI
    double _max;                         //!< max value, may be used in GUI
    double _value;                       //!< default and also current value
};

// DOUBLE
class IPLSHARED_EXPORT IPLProcessPropertyFloat : public IPLProcessProperty
{
public:
    IPLProcessPropertyFloat(IPLProcess* process, int position, const char* name, const char* description, float value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT,  float min=0.0f, float max=0.0f);

    float min() const                         { return _min; }
    float max() const                         { return _max; }
    float value() const                       { return _value; }
    void setValue(float value);
    virtual const char *type() const          { return "float"; }
    virtual SerializedData serialize() const;
    virtual void deserialize(const SerializedData &data);
    IPLProcessProperty *clone() const;

private:
    float _min;                         //!< min value, may be used in GUI
    float _max;                         //!< max value, may be used in GUI
    float _value;                       //!< default and also current value
};



// BOOL
class IPLSHARED_EXPORT IPLProcessPropertyBool : public IPLProcessProperty
{
public:
    IPLProcessPropertyBool(IPLProcess* process, int position, const char* name, const char* description, bool value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT);

    bool value() const                       { return _value; }
    void setValue(bool value);
    virtual const char *type() const         { return "bool"; }
    virtual SerializedData serialize() const;
    virtual void deserialize(const SerializedData &data);
    IPLProcessProperty *clone() const;

private:
    bool _value;                       //!< default and also current value
};

// STRING
class IPLSHARED_EXPORT IPLProcessPropertyString : public IPLProcessProperty
{
public:
    IPLProcessPropertyString(IPLProcess* process, int position, const char* name, const char* description, const std::string &value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT);

    std::string value() const                       { return _value; }
    void setValue(const std::string &value);
    void setValue(std::string &&value);
    virtual const char *type() const                { return "string"; }
    virtual SerializedData serialize() const;
    virtual void deserialize(const SerializedData &data);
    IPLProcessProperty *clone() const;

private:
    std::string _value;                       //!< default and also current value
};


// VECTOR<INT>
class IPLSHARED_EXPORT IPLProcessPropertyVectorInt : public IPLProcessProperty
{
public:
    IPLProcessPropertyVectorInt(IPLProcess* process, int position, const char* name, const char* description, const std::vector<int> &value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT);

    const std::vector<int> &value() const                       { return _value; }
    void setValue(const std::vector<int> &value);
    void setValue(std::vector<int> &&value);
    virtual const char *type() const                            { return "vector<int>"; }
    virtual SerializedData serialize() const;
    virtual void deserialize(const SerializedData &data);
    IPLProcessProperty *clone() const;

private:
    std::vector<int> _value;                       //!< default and also current value
};

// COLOR
class IPLSHARED_EXPORT IPLProcessPropertyColor : public IPLProcessProperty
{
public:
    IPLProcessPropertyColor(IPLProcess* process, int position, const char* name, const char* description, const IPLColor &value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT);

    const IPLColor &value() const                       { return _value; }
    void setValue(const IPLColor &value);
    void setValue(IPLColor &&value);
    virtual const char *type() const                    { return "color"; }
    virtual SerializedData serialize() const;
    virtual void deserialize(const SerializedData &data);
    IPLProcessProperty *clone() const;

private:
    IPLColor _value;                       //!< default and also current value
};


// POINT
class IPLSHARED_EXPORT IPLProcessPropertyPoint : public IPLProcessProperty
{
public:
    IPLProcessPropertyPoint(IPLProcess* process, int position, const char* name, const char* description, const IPLPoint &value, IPLProcessWidgetType widget = IPL_WIDGET_DEFAULT);

    const IPLPoint &value() const                       { return _value; }
    void setValue(const IPLPoint &value);
    void setValue(IPLPoint &&value);
    virtual const char *type() const                    { return "point"; }
    virtual SerializedData serialize() const;
    virtual void deserialize(const SerializedData &data);
    IPLProcessProperty *clone() const;

private:
    IPLPoint _value;                       //!< default and also current value
};

//! IPLProcessPropertyMap
typedef std::map<std::string, std::shared_ptr<IPLProcessProperty>> IPLProcessPropertyMap;


#endif // IPLPROCESSPROPERTY_H
