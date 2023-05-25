#include "val.h"
#include "expr.h"
#include "env.hpp"

// NumVal

NumVal::NumVal(int value) : value(value) {}

bool NumVal::is_true() {
    throw std::runtime_error("Num does not have bool");
}

bool NumVal::equals(PTR(Val) other) {
    PTR(NumVal) other_num = CAST(NumVal)(other);
    if (other_num) {
        return value == other_num->value;
    }
    return false;
}

std::string NumVal::to_string() {
    return std::to_string(value);
}

PTR(Val) NumVal::add_to(PTR(Val) other) {
    PTR(NumVal) other_num = CAST(NumVal)(other);
    if (other_num) {
        return NEW(NumVal)(value + other_num->value);
    }
    throw std::runtime_error ("NumVal: add_to: nullptr");
}

PTR(Val) NumVal::mult_with(PTR(Val) other) {
    PTR(NumVal) other_num = CAST(NumVal)(other);
    if (other_num) {
        return NEW(NumVal)(value * other_num->value);
    }
    throw std::runtime_error ("NumVal: mult_with: nullptr");
}

PTR(Val) NumVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error ("NumVal: call: method only for FunVal");
}

// BoolVal

BoolVal::BoolVal(bool value) : value(value) {}

bool BoolVal::equals(PTR(Val) other) {
    PTR(BoolVal) other_bool = CAST(BoolVal)(other);
    return other_bool && value == other_bool->value;
}

bool BoolVal::is_true() {
    return value;
}

PTR(Val) BoolVal::add_to(PTR(Val) other) {
    throw std::runtime_error("Cannot add boolean values.");
}

PTR(Val) BoolVal::mult_with(PTR(Val) other) {
    throw std::runtime_error("Cannot multiply boolean values.");
}

std::string BoolVal::to_string() {
    return value ? "_true" : "_false";
}

PTR(Expr) BoolVal::to_expr() {
    return NEW(BoolExpr)(value);
}

PTR(Val) BoolVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error ("BoolVal: call: method only for FunVal");
}

// FunVal

FunVal::FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env)
: formal_arg(formal_arg), body(body), env(env) {
}

bool FunVal::equals(PTR(Val) other) {
    PTR(FunVal) other_fun = CAST(FunVal)(other);
    return other_fun && formal_arg == other_fun->formal_arg && body->equals(other_fun->body);
}

bool FunVal::is_true() {
    throw std::runtime_error("FunVal: is_true: no boolean values to return");
}

std::string FunVal::to_string() {
    std::stringstream ss;
    ss << "_fun(" << formal_arg << ") " << body->to_string();
    return ss.str();
}

PTR(Val) FunVal::add_to(PTR(Val) other) {
    throw std::runtime_error("FunVal: add_to: Cannot add function values");
}

PTR(Val) FunVal::mult_with(PTR(Val) other) {
    throw std::runtime_error("FunVal: mult_with: Cannot multiply function values");
}

PTR(Val) FunVal::call(PTR(Val) actual_arg) {
    PTR(Env) new_env = NEW(ExtendedEnv)(formal_arg, actual_arg, env);
    return body->interp(new_env);
}
