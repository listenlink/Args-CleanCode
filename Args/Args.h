#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

enum ErrorCode {
    OK, MISSING_STRING, MISSING_INTEGER, INVALID_INTEGER, UNEXPECTED_ARGUMENT
};

class ParseException {
public:
    ParseException(const string& exception_msg):msg(exception_msg){}
    const string& message() { return msg; };
private:
    string msg;
};

class ArgsException {
public:
    ArgsException(const string& exception_msg) :msg(exception_msg) {}
    const string& message() { return msg; };
private:
    string msg;
};
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
    Args(string schema, vector<string> args);
    ~Args();

    void parse();
    bool parseSchema();
    void parseSchemaElement(string element);
    void validateSchemaElementId(char elementId);
    
    void parseBooleanSchemaElement(char elementId);
    void parseIntegerSchemaElement(char elementId);
    void parseStringSchemaElement(char elementId);

    bool isStringSchemaElement(string elementTail);
    bool isBooleanSchemaElement(string elementTail);
    bool isIntegerSchemaElement(string elementTail);

    bool parseArguments();
    void parseArgument(string arg);
    void parseElements(string arg);
    void parseElement(char argChar);

    bool setArgument(char argChar);

    int cardinality();
    string usage();
    string errorMessage();
    string unexpectedArgumentMessage();

    string getString(char arg);
    int getInt(char arg);
    bool getBoolean(char arg);
    bool has(char arg);

private:

    string schema;
    vector<string> args;
    set<char> unexpectedArguments = {};
    map<char, ArgumentMarshaler*> marshaler = {};
    set<char> argsFound = {};
    int currentArgument;
    char errorArgumentId = '\0';
    string errorParameter = "TILT";
    ErrorCode errorCode = ErrorCode::OK;
};
