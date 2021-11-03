CC = g++
CFLAGS = -g -Wall

all: wav

wav: main.o file.o ui.o effects.o
	$(CC) $(CFLAGS) -o wav main.o file.o ui.o effects.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

file.o: file.cpp file.h
	$(CC) $(CFLAGS) -c file.cpp


ui.o: ui.cpp ui.h
	$(CC) $(CFLAGS) -c ui.cpp

effects.o: effects.cpp effects.h
	$(CC) $(CFLAGS) -c effects.cpp

clean:
	rm *.o wav