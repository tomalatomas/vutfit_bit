CFLAGS= -Wall -Wextra -pedantic -pthread -lssl -lcrypto
PROJ=popcl
CC=g++


all: $(PROJ)

args.o: args.cpp args.hpp
	$(CC) $(CFLAGS) -c args.cpp -o args.o

connections.o: connections.cpp connections.hpp
	$(CC) $(CFLAGS) -c connections.cpp -o connections.o

$(PROJ).o: $(PROJ).cpp args.hpp connections.hpp
	$(CC) $(CFLAGS) -c $(PROJ).cpp -o $(PROJ).o

$(PROJ): $(PROJ).o args.o connections.o
	$(CC) $(CFLAGS) args.o connections.o $(PROJ).o -o $(PROJ)

clean:
	rm -f *.o $(PROJ) .downloaded 

