CC = gcc
CFLAGS = -Wall -g

all: gc debug_gc

gc: main.o hw4.o
	$(CC) $(CFLAGS) -o gc main.o hw4.o

debug_gc: debug_main.o hw4.o
	$(CC) $(CFLAGS) -o debug_gc debug_main.o hw4.o

main.o: main.c hw4.h
	$(CC) $(CFLAGS) -c main.c

debug_main.o: debug_main.c hw4.h
	$(CC) $(CFLAGS) -c debug_main.c

hw4.o: hw4.c hw4.h
	$(CC) $(CFLAGS) -c hw4.c

clean:
	rm -f *.o gc debug_gc
