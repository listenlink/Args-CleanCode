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
    Args(const string& schema, vector<string> args);
    ~Args();

    void parse();
    void parseSchema();
    void parseSchemaElement(const string& element);

    void parseArguments();
    void parseArgument(const string& arg);
    void parseElements(const string& arg);
    void parseElement(char argChar);

    bool setArgument(char argChar);

    int cardinality();
    string usage();

    string getString(char arg);
    int getInt(char arg);
    bool getBoolean(char arg);
    bool has(char arg);

private:

    void validateSchemaElementId(char elementId);

    bool isStringSchemaElement(const string& elementTail);
    bool isBooleanSchemaElement(const string& elementTail);
    bool isIntegerSchemaElement(const string& elementTail);
private:

    string schema;
    vector<string> args;
    map<char, unique_ptr<ArgumentMarshaler>> marshaler;
    set<char> argsFound = {};
    int currentArgument;
};
