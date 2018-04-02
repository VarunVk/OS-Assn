CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g

make: rec10.c
	$(CC) $(CFLAGS) -o rec10.o rec10.c -pthread

debug: rec10.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o rec10.o rec10.c -pthread

clean:
	rm rec10.o
