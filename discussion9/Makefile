CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g

make: rec9.c
	$(CC) $(CFLAGS) -o rec9.o rec9.c -pthread

debug: rec9.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o rec9.o rec9.c -pthread

clean:
	rm rec9.o
