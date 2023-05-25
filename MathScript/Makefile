# Name: Jowie Tan
# This makefile is to compile main.cpp cmdline.cpp and expr.cpp into a single executable
# The two arguments it takes is
# --test : Test if the command line is read.
# --help : List the available commands
# This is a loop so an example to run the program would be
# ./msdscript --help
# ./msdscript --test --test (This is to ensure that test is already read once)

CXX = clang++

SRCS = main.cpp cmdline.cpp expr.cpp val.cpp env.cpp parse.cpp testcases.cpp
OBJS = main.o cmdline.o expr.o val.o env.o parse.o testcases.o

CXXFLAGS = --std=c++14

msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o msdscript
	
cmdline.o: cmdline.cpp cmdline.h

main.o: main.cpp

expr.o: expr.h expr.cpp

parse.o: parse.hpp parse.cpp

.PHONY: gentest
gentest: test_generation.cpp exec.cpp 
	clang++ test_generation.cpp exec.cpp -o test_msdscript

.PHONY: test
test: msdscript
	./msdscript --test

.PHONY: clean
clean:
	rm -f *.o msdscript

.PHONY: cleantest
cleantest:
	rm -f *.o test_msdscript

.PHONY: doc
doc:
	cd documentation && doxygen

.PHONY: showdoc
showdoc:
	cd documentation/html && open index.html
