// Args.cpp : Defines the entry point for the console application.
//

#include "args.h"

Args::Args(string schema, vector<string> args): schema(schema), args(args) {
    parse();
}

Args::~Args(){}

void Args::parse() {
    parseSchema();
    parseArguments();
}

void Args::parseSchema() {
    if (schema.length() == 0)
        return;

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
    return;
}

void Args::parseSchemaElement(string element) {
    char elementId = element[0];
    string elementTail = element.substr(1);
    if (validateSchemaElementId(elementId)) {
        if (isBooleanSchemaElement(elementTail))
            parseBooleanSchemaElement(elementId);
        else if (isStringSchemaElement(elementTail))
            parseStringSchemaElement(elementId);
        else if (isIntegerSchemaElement(elementTail))
            parseIntegerSchemaElement(elementId);
        else
            throw ArgsException(ErrorCode::INVALID_FORMAT, elementId);
    }
    else
        throw ArgsException(ErrorCode::INVALID_FORMAT, elementId);
}

bool Args::validateSchemaElementId(char elementId) {
    return isalpha(elementId);
}
void Args::parseBooleanSchemaElement(char elementId) {
    marshaler[elementId] = make_unique<BoolArgumentMarshaler>();
}
void Args::parseIntegerSchemaElement(char elementId) {
    marshaler[elementId] = make_unique<IntegerArgumentMarshaler>();
}
void Args::parseStringSchemaElement(char elementId) {
    marshaler[elementId] = make_unique<StringArgumentMarshaler>();
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

void Args::parseArguments() {
    if (args.size() == 0)
        return;
    for (currentArgument = 0; static_cast<size_t>(currentArgument) < args.size(); currentArgument++)
    {
        string arg = args[currentArgument];
        parseArgument(arg);
    }
    return;
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
        throw ArgsException(ErrorCode::UNEXPECTED_ARGUMENT, argChar);
    }
}

bool Args::setArgument(char argChar) {
    auto marshalIter = marshaler.find(argChar);
    if (marshalIter == marshaler.end())
        return false;
    try {
        if (dynamic_cast<BoolArgumentMarshaler*>(marshalIter->second.get()))
            marshalIter->second->set("true");
        else {
            currentArgument++;
            marshalIter->second->set(args.at(currentArgument));
        }
    }
    catch (std::out_of_range e) {
        if(dynamic_cast<StringArgumentMarshaler*>(marshalIter->second.get()))
            throw ArgsException(ErrorCode::MISSING_STRING, argChar);
        else
            throw ArgsException(ErrorCode::MISSING_INTEGER, argChar);
    } 
    catch (std::invalid_argument e) {
        throw ArgsException(ErrorCode::INVALID_INTEGER, argChar, args.at(currentArgument));

    }
    return true;
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

string Args::getString(char arg) {
    auto am = marshaler[arg].get();
    try {
        return am == nullptr ? "" : am->get().String;
    }
    catch (exception e) {
        return "";
    }
}
int Args::getInt(char arg) {
    auto am = marshaler[arg].get();
    try {
        return am == nullptr? 0 : am->get().Integer;
    }
    catch (exception e) {
        return 0;
    }
}
bool Args::getBoolean(char arg) {
    auto am = marshaler[arg].get();
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


