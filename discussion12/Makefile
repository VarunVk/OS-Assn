CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g

make: server.c client.c
	$(CC) $(CFLAGS) -o serv.o server.c
  $(CC) $(CFLAGS) -o client.o client.c

debug: server.c client.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o serv.o server.c
	$(CC) $(CFLAGS) $(DBFLAGS) -o client.o client.c
clean:
	rm serv.o client.o
