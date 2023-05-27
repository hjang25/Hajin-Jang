# This is a makefile for 2 source files: digraph_analyzer.cpp, digraph_functions.c
# Typing 'make' or 'make digraph_analyzer' will create the executable named 'digraph_analyzer'

CC=g++
CFLAGS=-std=c++11 -pedantic -Wall -Wextra -g

digraph_analyzer: digraph_analyzer.o digraph_functions.o
	$(CC) -o digraph_analyzer digraph_analyzer.o digraph_functions.o

digraph_analyzer.o: digraph_analyzer.cpp digraph_functions.h
	$(CC) $(CFLAGS) -c digraph_analyzer.cpp

digraph_functions.o: digraph_functions.cpp digraph_functions.h
	$(CC) $(CFLAGS) -c digraph_functions.cpp

clean:
	rm -f *.o digraph_analyzer