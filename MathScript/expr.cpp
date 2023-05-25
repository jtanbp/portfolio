/**
 * \file expr.cpp
 * \brief Contains all the expression methods that are inherited by Num, Add, Mult and Var.
 */

#include "expr.h"
#include "val.h"

// Expr Function

/**
 * \brief prints out a simple string
 * \return string of entire expression
 */
std::string Expr::to_string() {
    std::stringstream st("");
    this->print(st);
    return st.str();
}

/**
 * \brief prints out a complicated string with precedence with brackets
 * \return string of entire expression
 */
std::string Expr::pretty_print_to_string() {
    return this->pretty_print();
}

// Num Functions

/**
 * \brief Sets value
 * \param val - integer
 */

Num::Num(int val) {
    this->val = NEW(NumVal)(val);
}

/**
 * \brief Compare Value
 * \param other - expression
 * \return Boolean result
 */

bool Num::equals(PTR(Expr) other) {
    if (PTR(Num) o = CAST(Num)(other)) {
        return val->equals(o->val);
    }
    return false;
}

/**
 * \brief Interprets the Num expression
 * \return int Value
 */

PTR(Val) Num::interp(PTR(Env) env) {
    return val;
}

/**
 * \brief prints out the value
 */

void Num::print(std::ostream &os) {
    os << val->to_string();
}

/**
 * \brief driver function for pretty print
 * \return string of the expression
 */
std::string Num::pretty_print() {
    std::stringstream st("");
    this->pretty_print_at(st, prec_none, 0, false);
    return st.str();
}

/**
 * \brief prints out the value
 */
void Num::pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side) {
    os << val->to_string();
}

// Add Functions

/**
 * \brief Sets value to add expression
 * \param lhs - expression
 * \param rhs - expression
 */

Add::Add(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

/**
 * \brief Compare Value
 * \param other - expression
 * \return Boolean result
 */

bool Add::equals(PTR(Expr) other) {
    if (PTR(Add) o = CAST(Add)(other)) {
        return lhs->equals(o->lhs) && rhs->equals(o->rhs);
    }
    return false;
}

/**
 * \brief Interprets the Add expression
 * \return Int add the value of the left expression and right expression
 */

PTR(Val) Add::interp(PTR(Env) env) {
    PTR(Val) lhs_val = lhs->interp(env);
    PTR(Val) rhs_val = rhs->interp(env);
    PTR(Val) result = lhs_val->add_to(rhs_val);
    return result;
}

/**
 * \brief prints out the Add expression as such "( lhs + rhs )"
 */
void Add::print(std::ostream &os) {
    os << "(";
    lhs->print(os);
    os << "+";
    rhs->print(os);
    os << ")";
}

/**
 * \brief driver function for pretty print
 * \return string of the expression
 */
std::string Add::pretty_print() {
    std::stringstream st("");
    this->pretty_print_at(st, prec_none, 0, false);
    return st.str();
}

/**
 * \brief prints out the Add expression as such "( lhs + rhs )" but with precedence depending on the order
 */
void Add::pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side) {
    precedence_t pre = prec_add;
    if (precedence >= prec_add){
        os << "(";
    }
    lhs->pretty_print_at(os, pre, pos, false);
    os << " + ";
    rhs->pretty_print_at(os, prec_none, pos, true);
    if (precedence >= prec_add){
        os << ")";
    }
}

// Mult Functions

/**
 * \brief Sets value to mult expression
 * \param lhs - expression
 * \param rhs - expression
 */

Mult::Mult(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

/**
 * \brief Compare Value
 * \param other - expression
 * \return Boolean result
 */

bool Mult::equals(PTR(Expr)other) {
    if (PTR(Mult) o = CAST(Mult)(other)) {
        return lhs->equals(o->lhs) && rhs->equals(o->rhs);
    }
    return false;
}

/**
 * \brief Interprets the Mult expression
 * \return Int times the value of the left expression and right expression
 */

PTR(Val) Mult::interp(PTR(Env) env) {
    PTR(Val) lhs_val = lhs->interp(env);
    PTR(Val) rhs_val = rhs->interp(env);
    PTR(Val) result = lhs_val->mult_with(rhs_val);
    return result;
}

/**
 * \brief prints out the Mult expression as such "( lhs * rhs )"
 */
void Mult::print(std::ostream &os) {
    os << "(";
    lhs->print(os);
    os << "*";
    rhs->print(os);
    os << ")";
}

/**
 * \brief driver function for pretty print
 * \return string of the expression
 */
std::string Mult::pretty_print() {
    std::stringstream st("");
    this->pretty_print_at(st, prec_none, 0, false);
    return st.str();
}

/**
 * \brief prints out the Mult expression as such "( lhs * rhs )" but with precedence depending on the order
 */

void Mult::pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side) {
    precedence_t pre = prec_mult;
    if (precedence >= prec_mult){
        os << "(";
    }
    lhs->pretty_print_at(os, pre, pos, false);
    os << " * ";
    rhs->pretty_print_at(os, prec_add, pos, true);
    if (precedence >= prec_mult){
        os << ")";
    }
}

// Var Functions

/**
 * \brief Sets String
 * \param value - string
 */

Var::Var(std::string value) {
    this->variable = value;
}

/**
 * \brief Compare Value
 * \param other - expression
 * \return Boolean result
 */

bool Var::equals(PTR(Expr) other) {
    if (PTR(Var) o = CAST(Var)(other)) {
        return variable == o->variable;
    }
    return false;
}

/**
 * \brief Interprets the variable expression
 * \return Runtime error as there is no value to interpret
 */

PTR(Val) Var::interp(PTR(Env) env) {
    return env->lookup(variable);
}

/**
 * \brief prints out the variable
 */
void Var::print(std::ostream &os) {
    os << variable;
}

/**
 * \brief driver function for pretty print
 * \return string of the expression
 */
std::string Var::pretty_print() {
    std::stringstream st("");
    this->pretty_print_at(st, prec_none, 0, false);
    return st.str();
}

/**
 * \brief prints out the variable
 */
void Var::pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side) {
    os << variable;
}

/****************************/

// LetExpr Functions

/**
 * \brief Sets name, value, and body for let expression
 * \param name - string representing the variable name
 * \param name_value - expression representing the value to bind to the variable
 * \param body - expression representing the body of the let expression
 */

Let::Let(std::string name, PTR(Expr) name_value, PTR(Expr) body) {
    this->name = name;
    this->name_value = name_value;
    this->body = body;
}

/**
 * \brief Compare Value
 * \param other - expression
 * \return Boolean result
 */
bool Let::equals(PTR(Expr) other) {
    if (PTR(Let) o = CAST(Let)(other)) {
        return name == o->name && name_value->equals(o->name_value) && body->equals(o->body);
    }
    return false;
}

/**
 * \brief Interprets the let expression by substituting the bound variable in the body with the value and interpreting the result
 * \return Int value of the let expression body
*/
PTR(Val) Let::interp(PTR(Env) env) {
    PTR(Val) value = name_value->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(name, value, env);
    return body->interp(new_env);
}

/**
 * \brief prints out the let expression with parentheses
 */
void Let::print(std::ostream &os) {
    os << "(_let " << name << "=";
    name_value->print(os);
    os << " _in ";
    body->print(os);
    os << ")";
}

/**
 * \brief driver function for pretty print
 * \return string of the expression
 */
std::string Let::pretty_print() {
    std::stringstream st("");
    this->pretty_print_at(st, prec_none, 0, false);
    return st.str();
}

/**
 * \brief prints out the let expression with parentheses and precedence
 */
void Let::pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side) {
    
    if ((precedence > prec_let && right_side) || body->equals(NEW(Var)(name))) {
            os << "(";
    }
        
    long posLet = os.tellp();
    // print "let"
    os << "_let " << name << " = ";
    name_value->pretty_print_at(os, prec_none, posLet, false);
    os << "\n";
    
    // calculate position for indentation
    long posIn = os.tellp();
    long space = posLet - pos;
    os << std::string(space, ' ') << "_in  ";
    
    // print "in"
    body->pretty_print_at(os, prec_none, posIn, true);
    if ((precedence > prec_let && right_side) || body->equals(NEW(Var)(name))) {
        os << ")";
    }
}

/****************************/

// Bool Expression

/**
 * \brief Sets value for the Bool expression
 * \param value - representing the boolean
 */
BoolExpr::BoolExpr(bool value) : value(value) {}

/**
 * \brief Compare Value
 * \param other - expression
 * \return Boolean result
 */
bool BoolExpr::equals(PTR(Expr) other) {
    PTR(BoolExpr) other_bool = CAST(BoolExpr)(other);
    return other_bool && value == other_bool->value;
}

/**
 * \brief Interprets the bool expression by returning the bool val in the expression
 * \return boolean value
*/
PTR(Val) BoolExpr::interp(PTR(Env) env) {
    return NEW(BoolVal)(value);
}

/**
 * \brief prints out the bool expression
 */
void BoolExpr::print(std::ostream &os) {
    os << (value ? "_true" : "_false");
}

/**
 * \brief driver function for pretty print
 * \return string of the expression
 */
std::string BoolExpr::pretty_print() {
    std::stringstream st("");
    this->pretty_print_at(st, prec_none, 0, false);
    return st.str();
}

/**
 * \brief prints out the boolean expression
 */
void BoolExpr::pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side) {
    print(os);
}

/****************************/

// If Expression
/**
 * \brief Sets condition, then clause, and else clause for if expression
 * \param cond - expression of the if expression
 * \param then_expr - expression representing the then clause of the expression
 * \param else_expr - expression representing the else clause of the expression
 */
IfExpr::IfExpr(PTR(Expr) cond, PTR(Expr) then_expr, PTR(Expr) else_expr)
    : cond(cond), then_expr(then_expr), else_expr(else_expr) {}


/**
 * \brief Compare Value
 * \param other - expression
 * \return Boolean result
 */
bool IfExpr::equals(PTR(Expr) other) {
    PTR(IfExpr) other_if = CAST(IfExpr)(other);
    return other_if && cond->equals(other_if->cond) &&
           then_expr->equals(other_if->then_expr) &&
           else_expr->equals(other_if->else_expr);
}

/**
 * \brief Interprets the if expression
 * \return Val of the interp (even if it is nested)
*/
PTR(Val) IfExpr::interp(PTR(Env) env) {
    PTR(Val) test_val = cond->interp(env);
    if (test_val->is_true()) {
        return then_expr->interp(env);
    } else {
        return else_expr->interp(env);
    }
}

/**
 * \brief prints out the if expression with parentheses
 */
void IfExpr::print(std::ostream &os) {
    os << "_if " << cond->to_string() << " _then " << then_expr->to_string() << " _else " << else_expr->to_string();
}

/**
 * \brief driver function for pretty print
 * \return string of the expression
 */
std::string IfExpr::pretty_print() {
    std::stringstream st("");
    this->pretty_print_at(st, prec_none, 0, false);
    return st.str();
}

/**
 * \brief prints out the if expression with parentheses and precedence
 */
void IfExpr::pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side) {
    // calculate position for indentation
    long space = os.tellp();
    os << std::string(space, ' ') << "_if  ";
    cond->pretty_print_at(os, prec_none, pos, false);
    os << "\n";
    os << std::string(space, ' ') << "_then  ";
    then_expr->pretty_print_at(os, prec_none, pos, false);
    os << "\n";
    os << std::string(space, ' ') << "_else  ";
    else_expr->pretty_print_at(os, prec_none, pos, false);
}

/****************************/

// Equals Expression
/**
 * \brief Sets left hand side and right hand side for equals expression
 * \param lhs - expression representing the left hand side of the expression
 * \param rhs - expression representing the right hand side of the expression
 */
EqExpr::EqExpr(PTR(Expr) lhs, PTR(Expr) rhs) : lhs(lhs), rhs(rhs) {}

/**
 * \brief Compare Value
 * \param other - expression
 * \return Boolean result
 */
bool EqExpr::equals(PTR(Expr) other) {
    PTR(EqExpr) other_eq = CAST(EqExpr)(other);
    return other_eq && lhs->equals(other_eq->lhs) && rhs->equals(other_eq->rhs);
}

/**
 * \brief Interprets the equals expression by comparing both expression within the equals expression
 * \return Boolean result
*/
PTR(Val) EqExpr::interp(PTR(Env) env) {
    PTR(Val) lhs_val = lhs->interp(env);
    PTR(Val) rhs_val = rhs->interp(env);
    return NEW(BoolVal)(lhs_val->equals(rhs_val));
}

/**
 * \brief prints out the Equals expression with parentheses
 */
void EqExpr::print(std::ostream &os) {
    os << lhs->to_string() << "==" << rhs->to_string();
}

/**
 * \brief driver function for pretty print
 * \return string of the expression
 */
std::string EqExpr::pretty_print() {
    std::stringstream st("");
    this->pretty_print_at(st, prec_none, 0, false);
    return st.str();
}

/**
 * \brief prints out the equals expression with parentheses and precedence
 */
void EqExpr::pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side) {
    if (precedence > prec_none) {
        os << "(";
    }
    lhs->pretty_print_at(os, prec_eq, pos, false);
    os << " == ";
    rhs->pretty_print_at(os, prec_eq, pos, true);
    if (precedence > prec_none) {
        os << ")";
    }
}

/****************************/

// Function Expression
/**
 * \brief Sets argument and body for function expression
 * \param arg - string representing argument
 * \param body - expression representing the right hand side of the expression
 */
FunExpr::FunExpr(std::string arg, PTR(Expr) body) : arg(arg), body(body) {}

/**
 * \brief Compare Value
 * \param other - expression
 * \return Boolean result
 */
bool FunExpr::equals(PTR(Expr) other) {
    PTR(FunExpr) other_eq = CAST(FunExpr)(other); 
    return other_eq && other_eq->arg == arg && other_eq->body->equals(body);
}

/**
 * \brief Interprets the function expression
 * \return Function value
*/
PTR(Val) FunExpr::interp(PTR(Env) env) {
    return NEW(FunVal)(arg, body, env);
}

/**
 * \brief prints out the function expression with parentheses
 */
void FunExpr::print(std::ostream &os) {
    os << "_fun (" << arg << ")\n";
    body->print(os);
}

/**
 * \brief driver function for pretty print
 * \return string of the expression
 */
std::string FunExpr::pretty_print() {
    std::stringstream st("");
    this->pretty_print_at(st, prec_none, 0, false);
    return st.str();
}

/**
 * \brief prints out the function expression with indentation, parentheses and precedence
 */
void FunExpr::pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side) {
    long space = os.tellp();
    os << "_f";
    space = os.tellp();
    os << "un (" << arg << ")\n";
    os << std::string(space, ' ');
    body->pretty_print_at(os, prec_none, space, false);
}

/****************************/

// Call Expression
/**
 * \brief Sets expression to intepret in the call expression
 * \param to_be_called - expression representing the desired expression
 * \param actual_argument - expression representing the actual argument of the expression
 */
CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_argument) : to_be_called(to_be_called), actual_argument(actual_argument) {}

/**
 * \brief Compare Value
 * \param other - expression
 * \return Boolean result
 */
bool CallExpr::equals(PTR(Expr) other) {
    PTR(CallExpr)other_eq = CAST(CallExpr)(other);
    return other_eq && other_eq->actual_argument->equals(actual_argument) && other_eq->to_be_called->equals(to_be_called);
}

/**
 * \brief Interprets the call expression
 * \return Replacing actual argument in the expression that is about to be called
*/
PTR(Val) CallExpr::interp(PTR(Env) env) {
    PTR(Val) to_be_called_val = to_be_called->interp(env);
    PTR(Val) actual_arg_val = actual_argument->interp(env);
    return to_be_called_val->call(actual_arg_val);
}

/**
 * \brief prints out the call expression with parentheses
 */
void CallExpr::print(std::ostream &os) {
    to_be_called->print(os);
    os << "(";
    actual_argument->print(os);
    os << ")";
}

/**
 * \brief driver function for pretty print
 * \return string of the expression
 */
std::string CallExpr::pretty_print() {
    std::stringstream st("");
    this->pretty_print_at(st, prec_none, 0, false);
    return st.str();
}

/**
 * \brief prints out the call expression with parentheses and precedence
 */
void CallExpr::pretty_print_at(std::ostream &os, precedence_t precedence, long pos, bool right_side) {
    long space = os.tellp();
    to_be_called->pretty_print_at(os, prec_none, space, false);
    os << "(";
    actual_argument->pretty_print_at(os, prec_none, space, false);
    os << ")";
}
