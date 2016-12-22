#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;
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

class Args {
public:
    Args(string schema, vector<string> args);
    ~Args();

    bool parse();
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

    bool isIntArg(char argChar);
    void setIntArg(char argChar);

    void setStringArg(char argChar);
    bool isStringArg(char argChar);

    void setBooleanArg(char argChar, bool value);
    bool isBooleanArg(char argChar);

    int cardinality();
    string usage();
    string errorMessage();
    string unexpectedArgumentMessage();

    string getString(char arg);
    int getInt(char arg);
    bool getBoolean(char arg);
    bool has(char arg);
    bool isValid();

private:
    enum ErrorCode {
        OK, MISSING_STRING, MISSING_INTEGER, INVALID_INTEGER, UNEXPECTED_ARGUMENT
    };

    string schema;
    vector<string> args;
    bool valid = true;
    set<char> unexpectedArguments = {};// new TreeSet<char>();
    map<char, bool> booleanArgs = {}; //        new HashMap<char, Boolean>();
    map<char, string> stringArgs = {}; //new HashMap<char, String>();
    map<char, int> intArgs = {}; // new HashMap<char, Integer>();
    set<char> argsFound = {};// new HashSet<char>();
    int currentArgument;
    char errorArgumentId = '\0';
    string errorParameter = "TILT";
    ErrorCode errorCode = ErrorCode::OK;

};
