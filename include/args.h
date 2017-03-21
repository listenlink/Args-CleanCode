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
    bool validateSchemaElementId(char elementId);

    bool isStringSchemaElement(string elementTail);
    bool isBooleanSchemaElement(string elementTail);
    bool isIntegerSchemaElement(string elementTail);
private:

    string schema;
    vector<string> args;
    map<char, unique_ptr<ArgumentMarshaler>> marshaler;
    set<char> argsFound = {};
    int currentArgument;
};
