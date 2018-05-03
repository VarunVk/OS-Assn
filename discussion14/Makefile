CC=gcc
CFLAGS=-std=gnu99 -lpthread
DBFLAGS=-g

make: rec14_1.c rec14_2.c
	$(CC) $(CFLAGS) -o rec14_1.o rec14_1.c
	$(CC) $(CFLAGS) -o rec14_2.o rec14_2.c

debug: rec14_1.c rec14_2.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o rec14_1_dbg.o rec14_1.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o rec14_2_dbg.o rec14_2.c

clean:
	rm rec14_1.o rec14_2.o
