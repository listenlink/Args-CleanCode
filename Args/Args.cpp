// Args.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "fmt\fmt_all.h"
#include "Args.h"

Args::Args(string schema, vector<string> args) {
    this->schema = schema;
    this->args = args;
    valid = parse();
}

Args::~Args(){}

bool Args::parse() {
    if (schema.length() == 0 && args.size() == 0) {
        return true;
    }
    parseSchema();
    try {
        parseArguments();
    }
    catch (ArgsException e) {
        e;
    }
    return valid;
}

bool Args::parseSchema() {
    std::string::size_type start_pos, find_pos;
    start_pos = find_pos = 0;
    do {
        find_pos = schema.find(',', start_pos);
        auto begin = schema.find_first_not_of(' ', start_pos);
        auto end = schema.find_last_not_of(" ,", find_pos);
        // find_pos is position of , so we need skip it by +1
        start_pos = find_pos + 1;
        parseSchemaElement(schema.substr(begin, end - begin + 1));
    } while (find_pos != std::string::npos);
    return true;
}
void Args::parseSchemaElement(string element) {
    char elementId = element[0];
    string elementTail = element.substr(1);
    validateSchemaElementId(elementId);
    if (isBooleanSchemaElement(elementTail))
        parseBooleanSchemaElement(elementId);
    else if (isStringSchemaElement(elementTail))
        parseStringSchemaElement(elementId);
    else if (isIntegerSchemaElement(elementTail)) {
        parseIntegerSchemaElement(elementId);
    }
    else {
        throw ParseException(fmt::sprintf("Argument: %c has invalid format : %s.", elementId, elementTail));
    }
}

void Args::validateSchemaElementId(char elementId) {
    if (!isalpha(elementId)){
        throw ParseException(fmt::sprintf("Bad character: %c",elementId) 
            + "in Args format: " + schema);
    }
}
void Args::parseBooleanSchemaElement(char elementId) {
    booleanArgs[elementId] = false;
}
void Args::parseIntegerSchemaElement(char elementId) {
    intArgs[elementId] = 0;
}
void Args::parseStringSchemaElement(char elementId) {
    stringArgs[elementId] = "";
}
bool Args::isStringSchemaElement(string elementTail) {
    return elementTail == "*";
}
bool Args::isBooleanSchemaElement(string elementTail) {
    return elementTail.length() == 0;
}
bool Args::isIntegerSchemaElement(string elementTail) {
    return elementTail == "#";
}

bool Args::parseArguments() {
    for (currentArgument = 0; static_cast<size_t>(currentArgument) < args.size(); currentArgument++)
    {
        string arg = args[currentArgument];
        parseArgument(arg);
    }
    return true;
}
void Args::parseArgument(string arg) {
    if (!arg.empty() && arg.front() == '-') {
        parseElements(arg);
    }
}
void Args::parseElements(string arg) {
        for (std::size_t i = 1; i < arg.length(); i++)
            parseElement(arg[i]);
    }

void Args::parseElement(char argChar) {
    if (setArgument(argChar))
        argsFound.insert(argChar);
    else {
        unexpectedArguments.insert(argChar);
        errorCode = ErrorCode::UNEXPECTED_ARGUMENT;
        valid = false;
    }
}

bool Args::setArgument(char argChar) {
    if (isBooleanArg(argChar))
        setBooleanArg(argChar, true);
    else if (isStringArg(argChar))
        setStringArg(argChar);
    else if (isIntArg(argChar))
        setIntArg(argChar);
    else
        return false;
    return true;
}

bool Args::isIntArg(char argChar) {
    return intArgs.find(argChar) != intArgs.end();
}

void Args::setIntArg(char argChar) {
    currentArgument++;
    string parameter = "";
    try {
        parameter = args.at(currentArgument);
        intArgs[argChar] = std::stoi(parameter);
    }
    catch (std::out_of_range e) {
        valid = false;
        errorArgumentId = argChar;
        errorCode = ErrorCode::MISSING_INTEGER;
        throw ArgsException("");
    }
    catch (std::invalid_argument e) {
        valid = false;
        errorArgumentId = argChar;
        errorParameter = parameter;
        errorCode = ErrorCode::INVALID_INTEGER;
        throw ArgsException("");
    }
}

void Args::setStringArg(char argChar) {
    currentArgument++;
    try {
        stringArgs[argChar] = args.at(currentArgument);
    }
    catch (std::out_of_range e) {
        valid = false;
        errorArgumentId = argChar;
        errorCode = ErrorCode::MISSING_STRING;
        throw ArgsException("");
    }
}

bool Args::isStringArg(char argChar) {
    return stringArgs.find(argChar) != stringArgs.end();
}
void Args::setBooleanArg(char argChar, bool value) {
    booleanArgs[argChar] = value;
}
bool Args::isBooleanArg(char argChar) {
    return booleanArgs.find(argChar) != booleanArgs.end();
}
int Args::cardinality() {
    return argsFound.size();
}
string Args::usage() {
    if (schema.length() > 0)
        return "-[" + schema + "]";
    else
        return "";
}
string Args::errorMessage() {
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
string Args::unexpectedArgumentMessage() {
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
string Args::getString(char arg) {
    auto iter = stringArgs.find(arg);
    return iter == stringArgs.end() ? string("") : iter->second;
}
int Args::getInt(char arg) {
    auto iter = intArgs.find(arg);
    return iter == intArgs.end() ? 0 : iter->second;
}
bool Args::getBoolean(char arg) {
    auto iter = booleanArgs.find(arg);
    return iter == booleanArgs.end() ? false : iter->second;
}
bool Args::has(char arg) {
    return argsFound.find(arg) != argsFound.end();
}
bool Args::isValid() {
    return valid;
}

