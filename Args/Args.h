#pragma once
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;
class ArgsException {
};

class Args {
private:

    enum ErrorCode {
        OK, MISSING_STRING, MISSING_INTEGER, INVALID_INTEGER, UNEXPECTED_ARGUMENT
    };
private:
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
    Args(string schema, vector<string> args);
    bool parse();
    bool parseSchema();
    void parseSchemaElement(string element);

    void validateSchemaElementId(char elementId);

    void parseBooleanSchemaElement(char elementId) {
        booleanArgs[elementId] = false;
    }
    void parseIntegerSchemaElement(char elementId) {
        intArgs[elementId] = 0;
    }
    void parseStringSchemaElement(char elementId) {
        stringArgs[elementId] = "";
    }
    bool isStringSchemaElement(string elementTail) {
        return elementTail == "*";
    }
    bool isBooleanSchemaElement(string elementTail) {
        return elementTail.length() == 0;
    }
    bool isIntegerSchemaElement(string elementTail) {
        return elementTail == "#";
    }
    bool parseArguments();
    void parseArgument(string arg);
    void parseElements(string arg);
    void parseElement(char argChar);

    bool setArgument(char argChar);
    bool isIntArg(char argChar) {
        return intArgs.find(argChar) != intArgs.end();
    }
    void setIntArg(char argChar);
    void setStringArg(char argChar);
    bool isStringArg(char argChar) {
        return stringArgs.find(argChar) != stringArgs.end();
    }
    void setBooleanArg(char argChar, bool value) {
        booleanArgs[argChar] = value;
    }
    bool isBooleanArg(char argChar) {
        return booleanArgs.find(argChar) != booleanArgs.end();
    }
    int cardinality() {
        return argsFound.size();
    }
    string usage() {
        if (schema.length() > 0)
            return "-[" + schema + "]";
        else
            return "";
    }
    string errorMessage(){
        switch (errorCode) {
        case OK:
            throw new string("TILT: Should not get here.");
        case UNEXPECTED_ARGUMENT:
            return unexpectedArgumentMessage();
        case MISSING_STRING:
            return string("Could not find string parameter for");
            /*
            return string.format("Could not find string parameter for -%c.",
                errorArgumentId);
                */
        case INVALID_INTEGER:
            return string("Argument -%c expects an integer but was");
            /*
            return string.format("Argument -%c expects an integer but was '%s'.",
                errorArgumentId, errorParameter);
                */
        case MISSING_INTEGER:
            return string("Could not find integer parameter for");
        }
        return "";
    }
    string unexpectedArgumentMessage() {
        /*
        StringBuffer message = new StringBuffer("Argument(s) -");
        for (char c : unexpectedArguments) {
            message.append(c);
        }
        message.append(" unexpected.");
        return message.toString();
        */
        return "unexpectedArgumentMessage";
    }
    string getString(char arg) {
        auto iter = stringArgs.find(arg);
        return iter == stringArgs.end() ? string("") : iter->second;
    }
    int getInt(char arg) {
        auto iter = intArgs.find(arg);
        return iter == intArgs.end() ? 0 : iter->second;
    }
    bool getBoolean(char arg) {
        auto iter = booleanArgs.find(arg);
        return iter == booleanArgs.end() ? false : iter->second;
    }
    bool has(char arg) {
        return argsFound.find(arg) != argsFound.end();
    }
    bool isValid() {
        return valid;
    }
};
