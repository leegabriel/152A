CC = g++
CFLAGS = -std=c++11 -Wall

all: simulation

event.o: event.cpp event.hpp
		$(CC) $(CFLAGS) -c event.cpp

packet.o: packet.cpp packet.hpp
		$(CC) $(CFLAGS) -c packet.cpp

main.o: main.cpp
		$(CC) $(CFLAGS) -c main.cpp

link:
		$(CC) event.o main.o packet.o -o simulation.out

simulation: event.o main.o packet.o link

clean:
		rm -f *.o *.out .*.*.swo .*.*.swp *.exe