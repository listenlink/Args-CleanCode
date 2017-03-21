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
        INVALID_INTEGER, 
        UNEXPECTED_ARGUMENT,
        INVALID_FORMAT
    };

    ArgsException(const ErrorCode& error_code = ErrorCode::OK, const char errorId = '\0', const string& params = "TILT") :
        errorCode(error_code), errorArgumentId(errorId), errorParameter(params) {
        setMessage();
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
            default:
                exceptionMsg = "";
        }
    }
    char errorArgumentId;
    string exceptionMsg;
    string errorParameter;
    ErrorCode errorCode;
};
