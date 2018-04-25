CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g

make: server.c client.c
	$(CC) $(CFLAGS) -o server.o server.c -pthread 
	$(CC) $(CFLAGS) -o client.o client.c -pthread

debug: server.c client.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o server.o server.c -pthread
	$(CC) $(CFLAGS) $(DBFLAGS) -o client.o client.c -pthread
clean:
	rm server.o client.o
