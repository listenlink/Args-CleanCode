// Args.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Args.h"

Args::Args(string schema, vector<string> args) {
    this->schema = schema;
    this->args = args;
    valid = parse();
}

bool Args::parse() {
    if (schema.length() == 0 && args.size() == 0)
        return true;
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
    /*
    for (string element : schema.split(",")) {
        if (element.length() > 0) {
            String trimmedElement = element.trim();
            parseSchemaElement(trimmedElement);
        }
    }
    */
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
        throw new string("Argument: %c has invalid format");
        /*
        throw new ParseException(
            String.format("Argument: %c has invalid format: %s.",
                elementId, elementTail), 0);
                */
    }
}

void Args::validateSchemaElementId(char elementId) {
    if (!isalpha(elementId)){
        throw new string(
            "Bad char: -------- in Args format: ");
    }
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
        throw new ArgsException();
    }
    catch (std::invalid_argument e) {
        valid = false;
        errorArgumentId = argChar;
        errorParameter = parameter;
        errorCode = ErrorCode::INVALID_INTEGER;
        throw new ArgsException();
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
        throw new ArgsException();
    }
}

int main()
{
    
    return 0;

}

