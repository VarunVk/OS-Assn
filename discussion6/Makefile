CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g

make: rec6.c
	$(CC) $(CFLAGS) -o rec6.o rec6.c

debug: rec6.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o rec6.o rec6.c

clean:
	rm rec6.o
