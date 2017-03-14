// Args.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "fmt\fmt_all.h"
#include "Args.h"

Args::Args(string schema, vector<string> args) {
    this->schema = schema;
    this->args = args;
    parse();
}

Args::~Args(){}

void Args::parse() {
    if (schema.length() == 0 && args.size() == 0) {
        return;
    }
    parseSchema();
    try {
        parseArguments();
    }
    catch (ArgsException e) {
        e;
    }
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
    marshaler[elementId] = new BoolArgumentMarshaler();
}
void Args::parseIntegerSchemaElement(char elementId) {
    marshaler[elementId] = new IntegerArgumentMarshaler();
}
void Args::parseStringSchemaElement(char elementId) {
    marshaler[elementId] = new StringArgumentMarshaler();
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
    }
}

bool Args::setArgument(char argChar) {
    auto m = marshaler[argChar];
    if (m == nullptr)
        return false;
    try {
        if (dynamic_cast<BoolArgumentMarshaler*>(m))
            m->set("true");
        else {
            currentArgument++;
            m->set(args.at(currentArgument));
        }
    }
    catch (ArgsException e) {
        errorArgumentId = argChar;
        throw e;
    }
    catch (std::out_of_range e) {
        errorArgumentId = argChar;
        if(dynamic_cast<StringArgumentMarshaler*>(m))
            errorCode = ErrorCode::MISSING_STRING;
        else 
            errorCode = ErrorCode::MISSING_INTEGER;
        throw ArgsException("");
    } 
    catch (std::invalid_argument e) {
        errorArgumentId = argChar;
        errorParameter = args.at(currentArgument);
        errorCode = ErrorCode::INVALID_INTEGER;
        throw ArgsException("");
    }
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
        throw string("TILT: Should not get here.");
    case UNEXPECTED_ARGUMENT:
        return unexpectedArgumentMessage();
    case MISSING_STRING:
        return string(fmt::sprintf("Could not find string parameter for : %c", errorArgumentId));
    case INVALID_INTEGER:
        return string(fmt::sprintf("Argument -%c expects an integer but was '%s'", errorArgumentId, errorParameter));
    case MISSING_INTEGER:
        return string(fmt::sprintf("Could not find integer parameter for : %c", errorArgumentId));
    }
    return "";
}
string Args::unexpectedArgumentMessage() {
   string message = "Argument(s) : ";
   string unexpected_chars(unexpectedArguments.begin(), unexpectedArguments.end());
   return message + unexpected_chars + " unexpected.";
}
string Args::getString(char arg) {
    auto am = marshaler[arg];
    try {
        return am == nullptr ? "" : am->get().String;
    }
    catch (exception e) {
        return "";
    }
}
int Args::getInt(char arg) {
    auto am = marshaler[arg];
    try {
        return am == nullptr? 0 : am->get().Integer;
    }
    catch (exception e) {
        return 0;
    }
}
bool Args::getBoolean(char arg) {
    auto am = marshaler[arg];
    try {
        return am == nullptr ? false : am->get().Boolean;
    }
    catch (exception e) {
        return false;
    }
}
bool Args::has(char arg) {
    return argsFound.find(arg) != argsFound.end();
}


