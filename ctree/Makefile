CC=g++
CFLAGS=-std=c++11 -pedantic -Wall -Wextra -g

cTreeTest: CTree.o CTreeTest.o
	$(CC) -o cTreeTest CTree.o CTreeTest.o

tTreeTest: TTreeTest.o
	$(CC) -o tTreeTest TTreeTest.o

cTreeTest.o: CTreeTest.cpp CTree.h
	$(CC) $(CFLAGS) -c CTreeTest.cpp

CTree.o: CTree.cpp CTree.h
	$(CC) $(CFLAGS) -c CTree.cpp

TTreeTest.o: TTreeTest.cpp Tree.h Tree.inc
	$(CC) $(CFLAGS) -c TTreeTest.cpp

clean:
	rm -f *.o cTreeTest
	rm -f *.o tTreeTest