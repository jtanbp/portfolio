//
//  env.hpp
//  ExpressionClass
//
//  Created by Jowie Tan on 4/11/23.
//

#ifndef env_hpp
#define env_hpp

#include "pointer.h"
#include "val.h"
#include <string>

CLASS(Env) {
public:
    virtual PTR(Val) lookup(std::string find_name) = 0;
    static PTR(Env) empty;
};

class EmptyEnv : public Env {
public:
    PTR(Val) lookup(std::string find_name);
};

class ExtendedEnv : public Env {
public:
    ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest);
    PTR(Val) lookup(std::string find_name);

private:
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;
};

#endif /* env_hpp */
