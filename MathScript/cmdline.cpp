#include "cmdline.h"

int handle_tests() {
    //Session to catch anything that isnt 0
    int exitnum = Catch::Session().run();
    if (exitnum != 0) {
        exit(1);
    }
    std::cout << "Tests passed" << std::endl;
    return 0;
}

int handle_interp() {
    std::string input;
    std::getline(std::cin, input);
    try {
        std::istringstream iss(input);
        PTR(Expr) e = parse_str(input);
        std::cout << e->interp(Env::empty) << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

int handle_print() {
    std::string input;
    std::getline(std::cin, input);
    try {
        std::istringstream iss(input);
        PTR(Expr) e = parse_expr(iss);
        std::cout << e->to_string() << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

int handle_pretty_print() {
    std::string input;
    std::getline(std::cin, input, (char)EOF);
    try {
        std::istringstream iss(input);
        PTR(Expr) e = parse_expr(iss);
        std::cout << e->pretty_print_to_string() << std::endl;
        return 0;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

int handle_invalid_arguments() {
    std::cerr << "Invalid command-line arguments" << std::endl;
    return 1;
}

run_mode_t use_arguments(int argc, char **argv) {
    if (argc == 2) {
        if (std::string(argv[1]) == "--help") {
            std::cout << "Allowed arguments:" << std::endl;
            std::cout << "--help: displays this help message" << std::endl;
            std::cout << "--test: runs tests" << std::endl;
            std::cout << "--interp: " << std::endl;
            std::cout << "--print: " << std::endl;
            std::cout << "--pretty_print: " << std::endl;
            exit(0);
        }
        else if (std::string(argv[1]) == "--test") {
            return do_test;
        }
        else if (std::string(argv[1]) == "--interp") {
            return do_interp;
        }
        else if (std::string(argv[1]) == "--print") {
            return do_print;
        }
        else if (std::string(argv[1]) == "--pretty-print") {
            return do_pretty_print;
        }
        else {
            std::cerr << "Error: unknown argument: " << argv[1] << std::endl;
            exit(1);
        }
    }
    return do_nothing;
}



