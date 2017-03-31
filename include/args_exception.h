#pragma once
#include <exception>
#include <string>
#include "fmt/printf.h"
using namespace std;

class ArgsException: public exception {
public:
    enum ErrorCode {
        OK, 
        MISSING_STRING, 
        MISSING_INTEGER, 
        MISSING_DOUBLE,
        INVALID_INTEGER,
        INVALID_DOUBLE,
        UNEXPECTED_ARGUMENT,
        INVALID_ARGUMEMNT_NAME,
        INVALID_FORMAT
    };

    ArgsException(const ErrorCode& error_code = ErrorCode::OK, const char errorId = '\0', const string& params = "TILT") :
        errorCode(error_code), errorArgumentId(errorId), errorParameter(params) {
        setMessage();
    }
    void setErrorParameter(const std::string& parameter) {
        errorParameter = parameter;
        setMessage();
    }
    void setErrorArgumentId(char id) {
        errorArgumentId = id;
        setMessage();
    }
    ErrorCode getErrorCode() const {
        return errorCode;
    }

    char getErrorArgumentId() const {
        return errorArgumentId;
    }

    const std::string& getErrorParameter() const {
        return errorParameter;
    }

    virtual const char* what() const throw() {
        return exceptionMsg.c_str();
    }
private:
    void setMessage() {
            switch (errorCode) {
            case OK:
                exceptionMsg =  string("TILT: Should not get here.");
                break;
            case UNEXPECTED_ARGUMENT:
                exceptionMsg = string(fmt::sprintf("Argument(s) : %c unexpected.", errorArgumentId));
                break;
            case MISSING_STRING:
                exceptionMsg =  string(fmt::sprintf("Could not find string parameter for : %c", errorArgumentId));
                break;
            case INVALID_INTEGER:
                exceptionMsg =  string(fmt::sprintf("Argument -%c expects an integer but was '%s'", errorArgumentId, errorParameter));
                break;
            case MISSING_INTEGER:
                exceptionMsg =  string(fmt::sprintf("Could not find integer parameter for : %c", errorArgumentId));
                break;
            case INVALID_FORMAT:
                exceptionMsg = string(fmt::sprintf("Argument: %c has invalid format.", errorArgumentId));
                break;
            case ErrorCode::INVALID_DOUBLE:
                exceptionMsg = string(fmt::sprintf("Argument -%c expects a double but was '%s'", errorArgumentId, errorParameter));
            case ErrorCode::MISSING_DOUBLE:
                exceptionMsg = string(fmt::sprintf("Could not find double parameter for : %c", errorArgumentId));
            default:
                exceptionMsg = "";
        }
    }
private:
    ErrorCode errorCode;
    char errorArgumentId;
    string exceptionMsg;
    string errorParameter;

};
