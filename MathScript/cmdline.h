/**
 * \file cmdline.h
 * \brief Contain functions to run program from terminal
 */

#ifndef CMDLINE_H
#define CMDLINE_H

#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "parse.hpp"
#include "expr.h"
#include <iostream>
#include <cstdlib>

typedef enum {
  do_nothing,
  do_test,
  do_interp,
  do_print,
  do_pretty_print,
} run_mode_t;

//void use_arguments(int argc, char** argv);

int handle_tests();

int handle_interp();

int handle_print();

int handle_pretty_print();

int handle_invalid_arguments();

run_mode_t use_arguments(int argc, char **argv);

#endif
