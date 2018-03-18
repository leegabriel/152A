CC = g++
CFLAGS = -std=c++11 -Wall

all: simulation

event.o: event.cpp event.hpp
		$(CC) $(CFLAGS) -c event.cpp

packet.o: packet.cpp packet.hpp
		$(CC) $(CFLAGS) -c packet.cpp

host.o: host.cpp host.hpp
		$(CC) $(CFLAGS) -c host.cpp

frame.o: frame.cpp frame.hpp
		$(CC) $(CFLAGS) -c frame.cpp		

main.o: main.cpp
		$(CC) $(CFLAGS) -c main.cpp

link:
		$(CC) event.o packet.o host.o frame.o main.o -o simulation.out

simulation: event.o packet.o host.o frame.o main.o link

clean:
		rm -f *.o *.out .*.*.swo .*.*.swp *.exe