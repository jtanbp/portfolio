/**
 * \file expr.h
 * \brief Contains the methods that would be defined in expr.cpp
 */

#ifndef EXPR_H
#define EXPR_H

#include <iostream>
#include <sstream>
#include <cstdlib>
#include "catch.h"
#include "pointer.h"
#include "env.hpp"

class Val;

/**
 * \brief defining precedence
 */

typedef enum {
    prec_none, // = 0;
    prec_eq, // = 1;
    prec_add,  // = 2;
    prec_mult,  // = 3;
    prec_let // = 4;
} precedence_t;

CLASS(Expr) {
public:
    virtual ~Expr() {}
    virtual bool equals(PTR(Expr) e) = 0;
    virtual PTR(Val) interp(PTR(Env) env) = 0;
    virtual void print(std::ostream &os) = 0;
    std::string to_string();
    std::string pretty_print_to_string();
    virtual std::string pretty_print() = 0;
    virtual void pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side) = 0;
};

class Num : public Expr {
private:
    PTR(Val) val;
public:
    Num (int val);
    bool equals(PTR(Expr) other);
    PTR(Val) interp(PTR(Env) env);
    void print(std::ostream &os);
    std::string pretty_print();
    void pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side);
};

class Add : public Expr {
private:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
public:
    Add(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) other);
    PTR(Val) interp(PTR(Env) env);
    void print(std::ostream &os);
    std::string pretty_print();
    void pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side);
};

class Mult : public Expr {
private:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
public:
    Mult(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) other);
    PTR(Val) interp(PTR(Env) env);
    void print(std::ostream &os);
    std::string pretty_print();
    void pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side);
};

class Var : public Expr {
private:
    std::string variable;
public:
    Var(std::string value);
    bool equals(PTR(Expr) other);
    PTR(Val) interp(PTR(Env) env);
    void print(std::ostream &os);
    std::string pretty_print();
    void pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side);
};

class Let : public Expr {
private:
    std::string name;
    PTR(Expr) name_value;
    PTR(Expr) body;
public:
    Let(std::string name, PTR(Expr) name_value, PTR(Expr) body);
    bool equals(PTR(Expr) other);
    PTR(Val) interp(PTR(Env) env);
    void print(std::ostream &os);
    std::string pretty_print();
    void pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right);
};

class BoolExpr : public Expr {
private:
    bool value;
public:
    BoolExpr(bool value);
    bool equals(PTR(Expr) other);
    PTR(Val) interp(PTR(Env) env);
    void print(std::ostream &os);
    std::string pretty_print();
    void pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side);
};

class IfExpr : public Expr {
private:
    PTR(Expr) cond;
    PTR(Expr) then_expr;
    PTR(Expr) else_expr;
public:
    IfExpr(PTR(Expr) cond, PTR(Expr) then_expr, PTR(Expr) else_expr);
    bool equals(PTR(Expr) other);
    PTR(Val) interp(PTR(Env) env);
    void print(std::ostream &os);
    std::string pretty_print();
    void pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side);
};

class EqExpr : public Expr {
private:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
public:
    EqExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) other);
    PTR(Val) interp(PTR(Env) env);
    void print(std::ostream &os);
    std::string pretty_print();
    void pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side);
};

class FunExpr : public Expr {
private:
    PTR(Expr) body;
public:
    FunExpr(std::string arg, PTR(Expr) body);
    std::string arg;
    bool equals(PTR(Expr) other);
    PTR(Val) interp(PTR(Env) env);
    void print(std::ostream &os);
    std::string pretty_print();
    void pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side);
};

class CallExpr : public Expr {
private:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_argument;
public:
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_argument);
    bool equals(PTR(Expr) other);
    PTR(Val) interp(PTR(Env) env);
    void print(std::ostream &os);
    std::string pretty_print();
    void pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side);
};



#endif
