#pragma once
#include <string>

typedef struct _object {
    bool Boolean = false;
    std::string String;
    int Integer;
    float Float;
    double Double;
    _object(bool v) : Boolean(v) {};
    _object(int v) : Integer(v) {};
    _object(std::string v) : String(v) {};

} object;

class ArgumentMarshaler {
public:
    virtual void set(std::string d) = 0;
    virtual object get() = 0;
    virtual ~ArgumentMarshaler() {};
};

class BoolArgumentMarshaler : public ArgumentMarshaler {
public:
    BoolArgumentMarshaler() : value(false) {}
    void set(std::string) { value.Boolean = true; }//TODO
    object get() { return value; }
    ~BoolArgumentMarshaler() {}
private:
    object value;
};

class StringArgumentMarshaler : public ArgumentMarshaler {
public:
    StringArgumentMarshaler() :value(std::string()) {}
    void set(std::string v) {
        value.String = v;
    }
    object get() { return value; }
    ~StringArgumentMarshaler() {}
private:
    object value;
};

class IntegerArgumentMarshaler : public ArgumentMarshaler {
public:
    IntegerArgumentMarshaler() :value(0) {}
    void set(std::string v) {
        value.Integer = std::stoi(v);
    }
    object get() { return value; }
    ~IntegerArgumentMarshaler() {}
private:
    object value;
};
