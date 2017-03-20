#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
#include "args_exception.h"
using namespace std;

#if 1
typedef struct _object {
    bool Boolean = false;
    string String;
    int Integer;
    float Float;
    double Double;
    _object(bool v) : Boolean(v) {};
    _object(int v) : Integer(v) {};
    _object(string v) : String(v) {};

} object;
#endif

class ArgumentMarshaler {
public:
    virtual void set(string d) = 0;
    virtual object get() = 0;
    virtual ~ArgumentMarshaler() {};
};

class BoolArgumentMarshaler : public ArgumentMarshaler {
public:
    BoolArgumentMarshaler() : value(false) {}
    void set(string v) { value.Boolean = true;}//TODO
    object get() { return value; }
    ~BoolArgumentMarshaler() {}
private:
    object value;
};

class StringArgumentMarshaler : public ArgumentMarshaler {
public:
    StringArgumentMarshaler() :value(string()) {}
    void set(string v) { 
        value.String = v;
    }
    object get() { return value; }
    ~StringArgumentMarshaler() {}
private:
    object value;
};

class IntegerArgumentMarshaler : public ArgumentMarshaler {
public:
    IntegerArgumentMarshaler() :value(0) {}
    void set(string v) {
            value.Integer = std::stoi(v);
    }
    object get() { return value; }
    ~IntegerArgumentMarshaler() {}
private:
    object value;
};

class Args {
public:
    using ErrorCode = ArgsException::ErrorCode;
    Args(string schema, vector<string> args);
    ~Args();

    void parse();
    void parseSchema();
    void parseSchemaElement(string element);
    bool validateSchemaElementId(char elementId);
    
    void parseBooleanSchemaElement(char elementId);
    void parseIntegerSchemaElement(char elementId);
    void parseStringSchemaElement(char elementId);

    bool isStringSchemaElement(string elementTail);
    bool isBooleanSchemaElement(string elementTail);
    bool isIntegerSchemaElement(string elementTail);

    void parseArguments();
    void parseArgument(string arg);
    void parseElements(string arg);
    void parseElement(char argChar);

    bool setArgument(char argChar);

    int cardinality();
    string usage();
    string unexpectedArgumentMessage();

    string getString(char arg);
    int getInt(char arg);
    bool getBoolean(char arg);
    bool has(char arg);

private:

    string schema;
    vector<string> args;
    map<char, ArgumentMarshaler*> marshaler = {};
    set<char> argsFound = {};
    int currentArgument;
};
