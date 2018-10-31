#This Makefile, written by Andrew H. Pometta, is the Makefile for the
#term-heval project.  It compiles the main executable from the source.

CXX = g++
CXXFLAGS += -O3 -std=c++11 -Wall -Wpedantic -pthread
RM = rm -rf
MSRC = PercentageToRange.cpp term-heval.cpp
#below taken and adapted from OMPEval makefile
OMPESRC = $(wildcard OMPEval/omp/*.cpp)
OMPEOBJ = ${OMPESRC:.cpp=.o}
LIB = OMPEval/lib
ARCH = $(LIB)/ompeval.a
EXEC = term-heval

#These two parts of the project do not have their dependencies included in
#the git repository, since they were not intended to be seen by the user.
#They were used for clerical/testing purposes only.
#Testy was a in-between program I used to generate certain files.
TSRC = testy.cpp
#The perctor driver was a program I used to test the percentagetorange class.
DSRC = PercentageToRange.cpp perctor_driver.cpp

#Compiles main executable and its various parts
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

driver: $(DSRC)
	$(CXX) $(CXXFLAGS) -o perctor-driver $^
