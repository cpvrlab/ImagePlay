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
    int position()                  { return _position; }
    const char* name()              { return _name; }
    const char* description()       { return _description; }
    IPLProcessPropertyType type()   { return _type; }
    virtual std::string toJson() = 0;
    virtual void fromJson(std::string) = 0;
    virtual IPLProcessProperty *clone() = 0;

protected:
    int _position;                  //!< Position in GUI
    const char* _name;              //!< Short name for GUI
    const char* _description;       //!< Short help for GUI
    IPLProcessPropertyType _type;   //!< Enum of GUI type
    IPLProcess*  _process;
};


// templates are not possible because of serialization issues

// INT
class IPLSHARED_EXPORT IPLProcessPropertyInt : public IPLProcessProperty
{
public:
    IPLProcessPropertyInt(IPLProcess* process, int position, const char* name, const char* description, IPLProcessPropertyType type, int value, int min=0, int max=0);

    int min()                         { return _min; }
    int max()                         { return _max; }
    int value()                       { return _value; }
    void setValue(int value);
    std::string toJson();
    void fromJson(std::string value);
    IPLProcessProperty *clone();

private:
    int _min;                         //!< min value, may be used in GUI
    int _max;                         //!< max value, may be used in GUI
    int _value;                       //!< default and also current value
};

// UNSIGNED INT
class IPLSHARED_EXPORT IPLProcessPropertyUnsignedInt: public IPLProcessProperty
{
public:
    IPLProcessPropertyUnsignedInt(IPLProcess* process, int position, const char* name, const char* description, IPLProcessPropertyType type, unsigned int value, unsigned int min=0, unsigned int max=0);

    unsigned int min()                         { return _min; }
    unsigned int max()                         { return _max; }
    unsigned int value()                       { return _value; }
    void setValue(unsigned int value);
    std::string toJson();
    void fromJson(std::string value);
    IPLProcessProperty *clone();

private:
    unsigned int _min;                         //!< min value, may be used in GUI
    unsigned int _max;                         //!< max value, may be used in GUI
    unsigned int _value;                       //!< default and also current value
};

// DOUBLE
class IPLSHARED_EXPORT IPLProcessPropertyDouble : public IPLProcessProperty
{
public:
    IPLProcessPropertyDouble(IPLProcess* process, int position, const char* name, const char* description, IPLProcessPropertyType type, double value, double min=0.0, double max=0.0);

    double min()                         { return _min; }
    double max()                         { return _max; }
    double value()                       { return _value; }
    void setValue(double value);
    std::string toJson();
    void fromJson(std::string value);
    IPLProcessProperty *clone();

private:
    double _min;                         //!< min value, may be used in GUI
    double _max;                         //!< max value, may be used in GUI
    double _value;                       //!< default and also current value
};

// DOUBLE
class IPLSHARED_EXPORT IPLProcessPropertyFloat : public IPLProcessProperty
{
public:
    IPLProcessPropertyFloat(IPLProcess* process, int position, const char* name, const char* description, IPLProcessPropertyType type, float value, float min=0.0f, float max=0.0f);

    float min()                         { return _min; }
    float max()                         { return _max; }
    float value()                       { return _value; }
    void setValue(float value);
    std::string toJson();
    void fromJson(std::string value);
    IPLProcessProperty *clone();

private:
    float _min;                         //!< min value, may be used in GUI
    float _max;                         //!< max value, may be used in GUI
    float _value;                       //!< default and also current value
};



// BOOL
class IPLSHARED_EXPORT IPLProcessPropertyBool : public IPLProcessProperty
{
public:
    IPLProcessPropertyBool(IPLProcess* process, int position, const char* name, const char* description, IPLProcessPropertyType type, bool value);

    bool min()                         { return _min; }
    bool max()                         { return _max; }
    bool value()                       { return _value; }
    void setValue(bool value);
    std::string toJson();
    void fromJson(std::string value);
    IPLProcessProperty *clone();

private:
    bool _min;                         //!< min value, may be used in GUI
    bool _max;                         //!< max value, may be used in GUI
    bool _value;                       //!< default and also current value
};

// STRING
class IPLSHARED_EXPORT IPLProcessPropertyString : public IPLProcessProperty
{
public:
    IPLProcessPropertyString(IPLProcess* process, int position, const char* name, const char* description, IPLProcessPropertyType type, std::string value);

    std::string min()                         { return _min; }
    std::string max()                         { return _max; }
    std::string value()                       { return _value; }
    void setValue(std::string value);
    std::string toJson();
    void fromJson(std::string value);
    IPLProcessProperty *clone();

private:
    std::string _min;                         //!< min value, may be used in GUI
    std::string _max;                         //!< max value, may be used in GUI
    std::string _value;                       //!< default and also current value
};


// VECTOR<INT>
class IPLSHARED_EXPORT IPLProcessPropertyVectorInt : public IPLProcessProperty
{
public:
    IPLProcessPropertyVectorInt(IPLProcess* process, int position, const char* name, const char* description, IPLProcessPropertyType type, std::vector<int> value);

    std::vector<int> min()                         { return _min; }
    std::vector<int> max()                         { return _max; }
    std::vector<int> value()                       { return _value; }
    void setValue(std::vector<int> value);
    std::string toJson();
    void fromJson(std::string value);
    IPLProcessProperty *clone();

private:
    std::vector<int> _min;                         //!< min value, may be used in GUI
    std::vector<int> _max;                         //!< max value, may be used in GUI
    std::vector<int> _value;                       //!< default and also current value
};

// COLOR
class IPLSHARED_EXPORT IPLProcessPropertyColor : public IPLProcessProperty
{
public:
    IPLProcessPropertyColor(IPLProcess* process, int position, const char* name, const char* description, IPLProcessPropertyType type, IPLColor value);

    IPLColor value()                       { return _value; }
    void setValue(IPLColor value);
    std::string toJson();
    void fromJson(std::string value);
    IPLProcessProperty *clone();

private:
    IPLColor _value;                       //!< default and also current value
};


// POINT
class IPLSHARED_EXPORT IPLProcessPropertyPoint : public IPLProcessProperty
{
public:
    IPLProcessPropertyPoint(IPLProcess* process, int position, const char* name, const char* description, IPLProcessPropertyType type, IPLPoint value);

    IPLPoint value()                       { return _value; }
    void setValue(IPLPoint value);
    std::string toJson();
    void fromJson(std::string value);
    IPLProcessProperty *clone();

private:
    IPLPoint _value;                       //!< default and also current value
};

//! IPLProcessPropertyMap
typedef std::map<std::string, std::unique_ptr<IPLProcessProperty>> IPLProcessPropertyMap;


#endif // IPLPROCESSPROPERTY_H
