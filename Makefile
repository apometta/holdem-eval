#This Makefile, written by Andrew H. Pometta, is the Makefile for the
#holdem-eval project.  It compiles the main executable from the source.

#To build the program, simply use "make" as the target in the directory
#with this Makefile.  The executable and all necessary dependencies should
#be compiled and built - the executable is "holdem-eval", which can be run
#in the directory with the command "./holdem-eval ...".  See README.md or run
#"./holdem-eval -h" for more information.

#To remove all built files and the executable, run "make clean".  To remove all
#dependencies that are not the executable, run "make clean-dependencies".

CXX = g++
CXXFLAGS += -O3 -std=c++11 -Wall -Wpedantic -pthread
RM = rm -rf
MSRC = PercentageToRange.cpp holdem-eval.cpp
#below taken and adapted from OMPEval makefile
OMPESRC = $(wildcard OMPEval/omp/*.cpp)
OMPEOBJ = ${OMPESRC:.cpp=.o}
LIB = OMPEval/lib
ARCH = $(LIB)/ompeval.a
EXEC = holdem-eval

all: $(EXEC)

$(EXEC): $(MSRC) $(ARCH)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(ARCH): $(OMPEOBJ) | $(LIB)
	ar rcs $@ $^

$(LIB):
	mkdir $@

clean: clean-dependencies
	$(RM) $(EXEC)

clean-dependencies:
	$(RM) $(ARCH) $(LIB) $(OMPEOBJ)
