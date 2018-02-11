CC = g++
CFLAGS = -std=c++11 -Wall

all: simulation

event.o: event.cpp event.hpp
		$(CC) $(CFLAGS) -c event.cpp

main.o: main.cpp
		$(CC) $(CFLAGS) -c main.cpp

link:
		$(CC) event.o main.o -o simulation.out

simulation: event.o main.o link

clean:
		rm -f *.o *.out .*.*.swo .*.*.swp *.exe