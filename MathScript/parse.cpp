//
//  parse.cpp
//  ExpressionClass
//
//  Created by Jowie Tan on 2/21/23.
//

#include "parse.hpp"

PTR(Expr) parse_str(std::string str) {
    std::istringstream is (str);
    return parse_expr(is);
}

PTR(Expr) parse_expr(std::istream &in) {
    PTR(Expr) e = parse_eq(in);
    skip_whitespaces(in);
    int c = in.peek();
    
    if (c == '+') {
        consume(in, '+');
        PTR(Expr)  rhs = parse_expr(in);
        return NEW(Add)(e, rhs);
    }
    else {
        return e;
    }
}

PTR(Expr) parse_eq(std::istream &in) {
    PTR(Expr) e = parse_addend(in);
    skip_whitespaces(in);
    int c = in.peek();
    if (c == '=') {
        consume(in, c);
        c = in.peek();
        if (c == '=') {
            consume(in, c);
            PTR(Expr) rhs = parse_expr(in);
            return NEW(EqExpr)(e, rhs);
        }
        else {
            throw std::runtime_error("invalid input");
        }
    }
    else {
        return e;
    }
}

PTR(Expr) parse_addend(std::istream &in) {
    PTR(Expr) e = parse_multicand(in);
    skip_whitespaces(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        PTR(Expr) rhs = parse_addend(in);
        return NEW(Mult)(e, rhs);
    }
    else {
        return e;
    }
}



PTR(Expr) parse_multicand(std::istream &in) {
    PTR(Expr) e = parse_inner(in);
    skip_whitespaces(in);
    while (in.peek() == '(') {
        consume(in, '(');
        PTR(Expr) actual_arg = parse_expr(in);
        consume(in, ')');
        e = NEW(CallExpr)(e, actual_arg);
    }
    return e;
}

PTR(Expr) parse_inner(std::istream &in) {
    skip_whitespaces(in);
    int c = in.peek();
    if ((c == '-') || isdigit(c)) {
        return parse_num(in);
    }
    else if (isalpha(c)){
        return parse_var(in);
    }
    else if (c == '_') {
        consume(in, c);
        skip_whitespaces(in);
        std::string keyword = parse_string(in);
        if (keyword == "let") {
            return parse_let(in);
        } else if (keyword == "true" || keyword == "false") {
            return parse_bool(in, keyword);
        } else if (keyword == "if") {
            return parse_if(in);
        } else if (keyword == "fun") {
            return parse_fun(in);
        } else {
            throw std::runtime_error("parse_inner: invalid input");
        }
    }
    else if (c == '(') {
        consume(in, '(');
        PTR(Expr) e = parse_expr(in);
        skip_whitespaces(in);
        c = in.get();
        if (c != ')'){
            throw std::runtime_error("bad input");
        }
        return e;
    }
    else {
        consume(in, c);
        throw std::runtime_error("bad input");
    }
}

PTR(Expr) parse_num(std::istream &in) {
    int num = 0;
    int count = 0;
    bool is_negative = false;
    if (in.peek() == '-'){
        is_negative = true;
        consume(in, '-');
        count++;
    }
    while (true) {
        int c = in.peek();
        if (isdigit(c)) {
            consume(in, c);
            num = num * 10 + (c - '0');
            count++;
        }
        else {
            break;
        }
    }
    if (is_negative) {
        num = -num;
    }
    if (is_negative & (count == 1)) {
        throw std::runtime_error("parse_num: invalid input");
    }
    
    return NEW(Num)(num);
}

std::string parse_string(std::istream &in) {
    std::string var = "";
    while (true) {
        int c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            var.append(std::string(1,char(c)));
        }
        else if (c == '_') {
            throw std::runtime_error("parse_string: invalid input of '_'");
        }
        else {
            break;
        }
    }
    return var;
}

PTR(Expr) parse_var(std::istream &in) {
    return NEW(Var)(parse_string(in));
}

PTR(Expr) parse_let(std::istream &in) {
    std::string name;
    PTR(Expr) name_value = NULL;
    PTR(Expr) body = NULL;
    skip_whitespaces(in);
    name = parse_string(in);
    skip_whitespaces(in);
    
    int c = in.peek();
    if (c == '=') {
        consume(in, c);
        name_value = parse_expr(in);
        skip_whitespaces(in);
    }
    else {
        throw std::runtime_error("parse_let: invalid input not '='");
    }
    
    c = in.peek();
    if (c == '_'){
        consume(in, c);
        skip_whitespaces(in);
        for (int i = 0; i < 2; i++) {
            int let = in.peek();
            // Input has to be _let
            if ((i == 0 & let != 'i')|| (i == 1 & let != 'n')) {
                throw std::runtime_error("parse_let: invalid input not 'in'");
            }
            consume(in, let);
        }
        skip_whitespaces(in);
        body = parse_expr(in);
    }
    else {
        throw std::runtime_error("parse_let: invalid input not '_'");
    }
    return NEW(Let)(name, name_value, body);
}

void consume(std::istream &in, int expectation) {
    int ch = in.get();
    if (ch != expectation) {
        throw std::runtime_error("consume mismatch");
    }
}

void skip_whitespaces(std::istream &in) {
    //skipping all the whitespaces
    int ch;
    while ((ch = in.peek()) && isspace(ch)) {
        consume(in, ch);
    }
}

PTR(Expr) parse_bool(std::istream &in, const std::string &keyword) {
    if (keyword == "true") {
        return NEW(BoolExpr)(true);
    } else if (keyword == "false") {
        return NEW(BoolExpr)(false);
    } else {
        throw std::runtime_error("invalid boolean keyword");
    }
}

PTR(Expr) parse_if(std::istream &in) {
    skip_whitespaces(in);
    PTR(Expr) condition = parse_expr(in);
    skip_whitespaces(in);
    std::string keyword = "";
    
    int c = in.peek();
    if (c == '_'){
        consume(in, c);
        keyword = parse_string(in);
        if (keyword != "then") {
            throw std::runtime_error("expected '_then' keyword");
        }
    }
    skip_whitespaces(in);
    PTR(Expr) true_branch = parse_expr(in);
    skip_whitespaces(in);
    c = in.peek();
    if (c == '_'){
        consume(in, c);
        keyword = parse_string(in);
        if (keyword != "else") {
            throw std::runtime_error("expected '_else' keyword");
        }
    }
    skip_whitespaces(in);
    PTR(Expr) false_branch = parse_expr(in);
    return NEW(IfExpr)(condition, true_branch, false_branch);
}

PTR(Expr) parse_fun(std::istream &in) {
    skip_whitespaces(in);
    std::string func;
    PTR(Expr) body = NULL;
    if (in.peek() == '(') {
        consume(in, '(');
        func = parse_string(in);
        if (in.peek() == ')'){
            consume(in, ')');
            skip_whitespaces(in);
            body = parse_expr(in);
        }
        
        
        else {
            throw std::runtime_error("parse_fun: expected ')' char");
        }
    }
    else {
        throw std::runtime_error("parse_fun: expected '(' char");
    }
    
    if (body == NULL) {
        throw std::runtime_error("parse_fun: body is NULL");
    }
    
    return NEW(FunExpr)(func, body);
}

