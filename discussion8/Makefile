CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g

make: rec8.c
	$(CC) $(CFLAGS) -o rec8.o rec8.c -pthread

debug: rec8.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o rec8.o rec8.c -pthread

clean:
	rm rec8.o
