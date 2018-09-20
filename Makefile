#This Makefile, written by Andrew H. Pometta, is the Makefile for the
#term-heval project.  It compiles the main executable from the source.
#Currently under construction.

MSRC = term-heval.cpp
EXEC = term-heval
OPTS = -std=c++11 -Wall -Wextra -Wpedantic

all: $(MSRC)
	g++ $(OPTS) $(MSRC) -o $(EXEC)

clean: $(EXEC)
	rm $(EXEC)
