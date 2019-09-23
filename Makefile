nash: main.o NAsh.o
	g++ -std=c++11 -g -Wall main.o nash.o -o nash
main.o: main.cpp NAsh.h
	g++ -std=c++11 -g -Wall -c main.cpp
NAsh.o: NAsh.h NAsh.cpp
	g++ -std=c++11 -g -Wall -c NAsh.cpp
clean: 
	rm *.o nash