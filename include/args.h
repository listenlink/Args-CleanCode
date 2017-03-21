#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
#include "args_exception.h"
#include "argument_marshaler.h"
using namespace std;

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

    string getString(char arg);
    int getInt(char arg);
    bool getBoolean(char arg);
    bool has(char arg);

private:

    string schema;
    vector<string> args;
    map<char, unique_ptr<ArgumentMarshaler>> marshaler;
    set<char> argsFound = {};
    int currentArgument;
};
