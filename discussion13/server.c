#define _BSD_SOURCE
#define NUM_ARGS 2

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERVER_PORT 4061
#define MAX_CONNECTIONS 100

struct bankAccount {

	double balance;
	pthread_mutex_t* mutex;
};

struct threadArg {

	int accountNumber;
	double change;
	int clientfd;
};

void threadFunction(void* args) {

	struct threadArg* arg = args;

	// TODO: Alter the specified account;
	
	// TODO: Write back to client.
	
	// Close client connection.
	close(arg->clientfd);
	
	free(args);
}

int main(int argc, char** argv) {

	if (argc > NUM_ARGS + 1) {

		printf("Wrong number of args, expected %d, given %d\n", NUM_ARGS, argc - 1);
		exit(1);
	}
	
	// Create a TCP socket.
	int sock = socket(AF_INET , SOCK_STREAM , 0);
	
	// Bind it to a local address.
	struct sockaddr_in servAddress;
	servAddress.sin_family = AF_INET;
	servAddress.sin_port = htons(SERVER_PORT);
	servAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sock, (struct sockaddr *) &servAddress, sizeof(servAddress));
	
	// We must now listen on this port.
	listen(sock, MAX_CONNECTIONS);
	
	// TODO: Init bank accounts.
	
	// TODO: Init threads.

	// A server typically runs infinitely, with some boolean flag to terminate.
	while (1) {
		
		// Now accept the incoming connections.
		struct sockaddr_in clientAddress;
		
		socklen_t size = sizeof(struct sockaddr_in);
		int clientfd = accept(sock, (struct sockaddr*) &clientAddress, &size);
		
		// TODO: Read request from the socket.

		// TODO: Run a thread to handle the request.
	}
	
	// Close the socket.
	close(sock);
}
