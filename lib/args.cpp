// Args.cpp : Defines the entry point for the console application.
//

#include "args.h"

Args::Args(const string& schema, vector<string> args):
  schema(schema), args(args) {
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
}

void Args::parseSchemaElement(const string& element) {
    char elementId = element[0];
    string elementTail = element.substr(1);
    validateSchemaElementId(elementId);

    if (isBooleanSchemaElement(elementTail))
        marshaler[elementId] = make_unique<BoolArgumentMarshaler>();
    else if (isStringSchemaElement(elementTail))
        marshaler[elementId] = make_unique<StringArgumentMarshaler>();
    else if (isIntegerSchemaElement(elementTail))
        marshaler[elementId] = make_unique<IntegerArgumentMarshaler>();
    else
        throw ArgsException(ErrorCode::INVALID_FORMAT, elementId);
}

void Args::validateSchemaElementId(char elementId) {
    if(!isalpha(elementId))
        throw ArgsException(ErrorCode::INVALID_ARGUMEMNT_NAME, elementId);
}

bool Args::isStringSchemaElement(const string& elementTail) {
    return elementTail == "*";
}
bool Args::isBooleanSchemaElement(const string& elementTail) {
    return elementTail.length() == 0;
}
bool Args::isIntegerSchemaElement(const string& elementTail) {
    return elementTail == "#";
}

void Args::parseArguments() {
    for (currentArgument = args.begin(); currentArgument != args.end(); currentArgument++) {
        parseArgument(*currentArgument);
    }
}

void Args::parseArgument(const string& arg) {
    if (!arg.empty() && arg.front() == '-')
        parseElements(arg);
}

void Args::parseElements(const string& arg) {
    for (std::size_t i = 1; i < arg.length(); i++)
        parseElement(arg[i]);
}

void Args::parseElement(char argChar) {
    auto argMarshalerIter = marshaler.find(argChar);
    if (argMarshalerIter == marshaler.end()) {
        throw ArgsException(ArgsException::ErrorCode::UNEXPECTED_ARGUMENT, argChar);
    }

    try {
        auto lastArgParsed = argMarshalerIter->second->set(currentArgument, args.end());
        currentArgument += lastArgParsed - currentArgument;
    }
    catch (ArgsException & e) {
        e.setErrorArgumentId(argChar);
        throw e;
    }
    argsFound.insert(argChar);
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


