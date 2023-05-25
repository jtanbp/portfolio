/**
 * \mainpage MSDScript
 * \author Jowie Tan
 * \date 02-07-2023
 */

#include "catch.h"
#include "cmdline.h"
#include "expr.h"

int main (int argc, char** argv) {
    
    //Testing statement for branching test
    //use_arguments(argc, argv);
    try {
        run_mode_t mode = use_arguments(argc, argv);
        switch (mode) {
            case do_test: {
                return handle_tests();
            }
            case do_interp: {
                return handle_interp();
            }
            case do_print: {
                return handle_print();
            }
            case do_pretty_print: {
                return handle_pretty_print();
            }
            default: {
                return handle_invalid_arguments();
            }
        }
        return 0;
    } catch (std::runtime_error exn) {
        std::cerr << exn.what() << "\n";
        return 1;
    }
}




