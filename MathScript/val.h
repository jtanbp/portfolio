#ifndef VAL_H
#define VAL_H

#include <string>
#include "pointer.h"

class Env;
class Expr;

CLASS(Val) {
public:
    virtual bool is_true() = 0;
    virtual bool equals(PTR(Val) other) = 0;
    virtual std::string to_string() = 0;
    virtual PTR(Val) add_to(PTR(Val) other) = 0;
    virtual PTR(Val) mult_with(PTR(Val) other) = 0;
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
};

class NumVal : public Val {
private:
    
public:
    NumVal(int value);
    int value;
    bool is_true();
    bool equals(PTR(Val) other);
    std::string to_string();
    PTR(Val) add_to(PTR(Val) other);
    PTR(Val) mult_with(PTR(Val) other);
    PTR(Val) call(PTR(Val) actual_arg);
};

class BoolVal : public Val {
private:
    
public:
    BoolVal(bool value);
    bool value;
    bool is_true();
    PTR(Expr) to_expr();
    bool equals(PTR(Val) other);
    std::string to_string();
    PTR(Val) add_to(PTR(Val) other);
    PTR(Val) mult_with(PTR(Val) other);
    PTR(Val) call(PTR(Val) actual_arg);
};

class FunVal : public Val {
private:
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;
public:
    FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env);
    bool is_true();
    bool equals(PTR(Val) other);
    std::string to_string();
    PTR(Val) add_to(PTR(Val) other);
    PTR(Val) mult_with(PTR(Val) other);
    PTR(Val) call(PTR(Val) actual_arg);
};

#endif
