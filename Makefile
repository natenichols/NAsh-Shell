nash: main.o NAsh.o
	g++ -std=c++11 -g -Wall main.o NAsh.o -o nash
	g++ -std=c++11 -g -Wall main.o NAsh.o -o quash
main.o: main.cpp NAsh.h
	g++ -std=c++11 -g -Wall -c main.cpp
NAsh.o: NAsh.h NAsh.cpp
	g++ -std=c++11 -g -Wall -c NAsh.cpp
clean: 
	rm *.o nash quash
