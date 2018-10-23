#This Makefile, written by Andrew H. Pometta, is the Makefile for the
#term-heval project.  It compiles the main executable from the source.

CXX = g++
CXXFLAGS += -O3 -std=c++11 -Wall -Wpedantic -pthread
RM = rm -rf
MSRC = term-heval.cpp
#below taken and adapted from OMPEval makefile
OMPESRC = $(wildcard OMPEval/omp/*.cpp)
OMPEOBJ = ${OMPESRC:.cpp=.o}
LIB = OMPEval/lib
TSRC = testy.cpp
ARCH = $(LIB)/ompeval.a
EXEC = term-heval

all: $(EXEC)

$(EXEC): $(MSRC) $(ARCH)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(ARCH): $(OMPEOBJ) | $(LIB)
	ar rcs $@ $^

$(LIB):
	mkdir $@

clean:
	$(RM) $(EXEC) $(ARCH) $(LIB) $(OMPEOBJ) testy

testy: $(TSRC) $(ARCH)
	$(CXX) $(CXXFLAGS) -o $@ $^
