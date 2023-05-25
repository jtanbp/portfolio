//
//  parse.hpp
//  ExpressionClass
//
//  Created by Jowie Tan on 2/21/23.
//

#ifndef parse_hpp
#define parse_hpp

#include <stdio.h>
#include "expr.h"
#include "val.h"

PTR(Expr) parse_str(std::string s);
PTR(Expr) parse_expr(std::istream& in);
PTR(Expr) parse_addend(std::istream &in);
PTR(Expr) parse_inner(std::istream &in);
PTR(Expr) parse_multicand(std::istream &in);
PTR(Expr) parse_num(std::istream &in);
std::string parse_string(std::istream &in);
PTR(Expr) parse_var(std::istream &in);
PTR(Expr) parse_let(std::istream &in);
void consume(std::istream &in, int expectation);
void skip_whitespaces(std::istream &in);
PTR(Expr) parse_bool(std::istream &in, const std::string &keyword);
PTR(Expr) parse_if(std::istream &in);
PTR(Expr) parse_eq(std::istream &in);
PTR(Expr) parse_fun(std::istream &in);

#endif /* parse_hpp */
