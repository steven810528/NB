all: c  main.o
	g++ main.o -o a
main.o: main.cpp
	g++ -c main.cpp  
c:
	rm *.o
