#pragma once
#include <string>

typedef struct _object {
    bool Boolean = false;
    std::string String;
    int Integer;
    double Double;
    _object(bool v) : Boolean(v) {};
    _object(int v) : Integer(v) {};
    _object(const std::string& v) : String(v) {};
    _object(double v) : Double(v) {};
} object;

using ArgumentStringsType = std::vector<std::string>;
using ArgumentStringIterator = ArgumentStringsType::iterator;

class ArgumentMarshaler {
public:
    virtual ArgumentStringIterator set(const ArgumentStringIterator& begin, const ArgumentStringIterator& end) = 0;
    virtual object get() = 0;
    virtual ~ArgumentMarshaler() {};
};

class BoolArgumentMarshaler : public ArgumentMarshaler {
public:
    BoolArgumentMarshaler() : value(false) {}
    ArgumentStringIterator set(const ArgumentStringIterator& begin, const ArgumentStringIterator& end) {
        if (begin >= end) {
            assert(false);
        }
        value.Boolean = true;
        return begin;
    }
    object get() { return value; }
    ~BoolArgumentMarshaler() {}
private:
    object value;
};

class StringArgumentMarshaler : public ArgumentMarshaler {
public:
    StringArgumentMarshaler() :value(std::string()) {}
    ArgumentStringIterator set(const ArgumentStringIterator& begin, const ArgumentStringIterator& end) {
        auto stringIt = begin + 1;
        if (stringIt >= end) {
            throw ArgsException(ArgsException::ErrorCode::MISSING_STRING);
        }
        value.String = *stringIt;
        return stringIt;
    }
    object get() { return value; }
    ~StringArgumentMarshaler() {}
private:
    object value;
};

class IntegerArgumentMarshaler : public ArgumentMarshaler {
public:
    IntegerArgumentMarshaler() :value(0) {}
    ArgumentStringIterator set(const ArgumentStringIterator& begin, const ArgumentStringIterator& end) {
        auto intIt = begin + 1;
        if (intIt >= end) {
            throw ArgsException(ArgsException::ErrorCode::MISSING_INTEGER);
        }

        try {
            value.Integer = std::stoi(*intIt);
        }
        catch (std::invalid_argument) {
            auto e = ArgsException(ArgsException::ErrorCode::INVALID_INTEGER);
            e.setErrorParameter(*intIt);
            throw e;
        }

        return intIt;
    }
    object get() { return value; }
    ~IntegerArgumentMarshaler() {}
private:
    object value;
};

class DoubleArgumentMarshaler : public ArgumentMarshaler {
public:
    DoubleArgumentMarshaler() :value(0) {}
    ArgumentStringIterator set(const ArgumentStringIterator& begin, const ArgumentStringIterator& end) {
        auto DoubleIt = begin + 1;
        if (DoubleIt >= end) {
            throw ArgsException(ArgsException::ErrorCode::MISSING_DOUBLE);
        }

        try {
            value.Double = std::stod(*DoubleIt);
        }
        catch (std::invalid_argument) {
            auto e = ArgsException(ArgsException::ErrorCode::INVALID_DOUBLE);
            e.setErrorParameter(*DoubleIt);
            throw e;
        }

        return DoubleIt;
    }
    object get() { return value; }
    ~DoubleArgumentMarshaler() {}
private:
    object value;
};
