#pragma once
#include <string>
using namespace std;
class ArgsException {
public:
    ArgsException(const string& exception_msg) : exception_msg(exception_msg) {}
    const string& message() { return exception_msg; };
private:
    string exception_msg;
};