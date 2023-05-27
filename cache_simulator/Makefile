#
# Jiwon Moon, Hajin Jang
#

CC = g++
CFLAGS = -std=c++11 -Wextra -Wall -pedantic
DBGFLAGS = -g

csim: cache_main.o cache_simulator.o
	$(CC) $(CFLAGS) $(DBGFLAGS) cache_simulator.o cache_main.o -o csim 

cache_main.o: cache_main.cpp
	$(CC) $(CFLAGS) $(DBGFLAGS) -c cache_main.cpp -o cache_main.o 

cache_simulator.o: cache_simulator.cpp cache_simulator.h
	$(CC) $(CFLAGS) $(DBGFLAGS) -c cache_simulator.cpp -o cache_simulator.o

# Use this target to create a zipfile that you can submit to Gradescope
.PHONY: solution.zip
solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *.h *.cpp

clean :
	rm -f *.o csim solution.zip \
		depend.mak solution.zip

depend.mak :
	touch $@

depend :
	gcc -M $(C_SRCS) > depend.mak

include depend.mak