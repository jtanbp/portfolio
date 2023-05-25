//
//  test_generation.cpp
//  ExpressionClass
//
//  Created by Jowie Tan on 2/28/23.
//

#include <stdio.h>
#include <iostream>
#include "exec.h"

const int MAX_RECUR = 5;

std::string random_expr_string(bool var_exist = false, int max_recursion = 0) {
    
    if (max_recursion >= MAX_RECUR){
        return std::to_string(rand() % 1000 + 1);
    }
    
    int r = rand() % 10;
    if (r < 5) { // 50% of the time, return a number or var
        if (r < 1 && var_exist) { // 20% of 50% of the time, return var if var exist
            return "x";
        } else { // 80% of 50% of the time, return a number
            return std::to_string(rand() % 1000 + 1); // number from 1 - 100000
        }
    } else if (r < 6) { // 10% of the time, return a let expression
        std::string var_name = "x";
        std::string expr1 = random_expr_string(false, max_recursion + 1);
        std::string expr2 = random_expr_string(true, max_recursion + 1);
        return "_let " + var_name + " = " + expr1 + " _in " + expr2;
    } else { // 40% of the time, return an arithmetic expression
        std::string op;
        if (rand() % 2 == 0) { // 50/50 on + or *
            op = "+";
        } else {
            op = "*";
        }
        std::string expr1 = random_expr_string(var_exist, max_recursion + 1);
        std::string expr2 = random_expr_string(var_exist, max_recursion + 1);
        return "(" + expr1 + " " + op + " " + expr2 + ")";
    }
}

void runningTests(char**& argv) {
    const char * const interp_argv[] = { argv[1], "--interp" };
    const char * const print_argv[] = { argv[1], "--print" };
    for (int i = 0; i < 100; i++) {
            std::string in = random_expr_string(false, 0);
            std::cout << "Trying"<< in << "\n";
            ExecResult interp_result = exec_program(2, interp_argv, in);
            ExecResult print_result = exec_program(2, print_argv, in);
            ExecResult interp_again_result = exec_program(2, interp_argv, print_result.out);
            if (interp_again_result.out != interp_result.out)
                throw std::runtime_error("different result for printed");
    }
}

void runCompare(char**& argv) {
    //interp compare
    const char * const interp1_argv[] = { argv[1], "--interp" };
    const char * const interp2_argv[] = { argv[2], "--interp" };
    
    for (int i = 0; i < 33; i++) {
        std::string in = random_expr_string(false, 0);
        std::cout << "Trying"<< in << "\n";
        ExecResult interp1_result = exec_program(2, interp1_argv, in);
        ExecResult interp2_result = exec_program(2, interp2_argv, in);
        
        if (interp1_result.out != interp2_result.out) {
            throw std::runtime_error("different interp result \na: " + interp1_result.out + "\nb:" + interp2_result.out);
        }
        if (interp1_result.exit_code != 0) {
            throw std::runtime_error(interp1_result.err);
        } else if (interp2_result.exit_code != 0) {
            throw std::runtime_error(interp2_result.err);
        }
    }
    
    //print compare
    const char * const print1_argv[] = { argv[1], "--print" };
    const char * const print2_argv[] = { argv[2], "--print" };
    
    for (int i = 0; i < 33; i++) {
        std::string in = random_expr_string(false, 0);
        std::cout << "Trying"<< in << "\n";
        ExecResult print1_result = exec_program(2, print1_argv, in);
        ExecResult print2_result = exec_program(2, print2_argv, in);
        
        if (print1_result.out != print2_result.out) {
            throw std::runtime_error("different print result\n a:\n" + print1_result.out + "\n b:\n" + print2_result.out);
        }
        if (print1_result.exit_code != 0) {
            throw std::runtime_error(print1_result.err);
        } else if (print2_result.exit_code != 0) {
            throw std::runtime_error(print2_result.err);
        }
    }
    
    //print compare
    const char * const p_print1_argv[] = { argv[1], "--pretty-print" };
    const char * const p_print2_argv[] = { argv[2], "--pretty-print" };
    
    for (int i = 0; i < 33; i++) {
        std::string in = random_expr_string(false, 0);
        std::cout << "Trying"<< in << "\n";
        ExecResult p_print1_result = exec_program(2, p_print1_argv, in);
        ExecResult p_print2_result = exec_program(2, p_print2_argv, in);
        
        if (p_print1_result.out != p_print2_result.out) {
            throw std::runtime_error("different pretty print result\n a:\n" + p_print1_result.out + "\n b:\n" + p_print2_result.out);
        }
        if (p_print1_result.exit_code != 0) {
            throw std::runtime_error(p_print1_result.err);
        } else if (p_print2_result.exit_code != 0) {
            throw std::runtime_error(p_print2_result.err);
        }
    }
}

int main(int argc, char **argv) {
    srand(time(0));
    if (argc == 2) {
        runningTests(argv);
    }
    else if (argc == 3) {
        runCompare(argv);
    }
    return 0;
}

//int main(int argc,  char** argv) {
//    const char * const interp_argv[] = {"msdscript", "--interp"};
//    const char * const print_argv[] = {"msdscript", "--print"};
//    //const char * const p_argv[] = {"msdscript", "--interp"};
//
//    for (int i = 0; i < 100; i++){
//        std::string in = random_expr_string();
//        std::cout << "Trying" << in << "\n";
//
//        ExecResult interp_result = exec_program(2, interp_argv, in);
//        ExecResult print_result = exec_program(2, print_argv, in);
//
//        ExecResult interp_again_result = exec_program(2, interp_argv, print_result.out);
//
//        if (interp_again_result.out != interp_result.out){
//            throw std::runtime_error("different result for printed");
//        }
//
//        if (interp_again_result.exit_code != 0) {
//            std::cerr << "non-zero exit: " << interp_again_result.exit_code << "\n";
//        }
////    if (wc_result.out != "   3\n"){ //this is where we e
////        std::cerr << "bad wc results\n";
////    }
//    }
//    return 0;
//}


