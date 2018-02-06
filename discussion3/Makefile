CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g

make: rec3.c
	$(CC) $(CFLAGS) -o comp.out rec3.c

debug: btree.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o comp.out rec3.c

clean:
	rm comp.out
