CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g

make: rec11.c
	$(CC) $(CFLAGS) -o rec11.o rec11.c -pthread

debug: rec11.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o rec11.o rec11.c -pthread

clean:
	rm rec11.o
