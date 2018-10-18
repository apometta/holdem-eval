#This Makefile, written by Andrew H. Pometta, is the Makefile for the
#term-heval project.  It compiles the main executable from the source.
#Currently under construction.

TEST = test.cpp #not related to project, just a testing area for random ideas
MSRC = term-heval.cpp
EXEC = term-heval
OPTS = -std=c++11 -Wall -Wextra -Wpedantic

all: $(MSRC)
	g++ $(OPTS) $(MSRC) -o $(EXEC)

clean: $(EXEC)
	rm $(EXEC)

test: $(TEST)
	g++ $(OPTS) $(TEST) -o test
