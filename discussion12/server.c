#define _BSD_SOURCE
#define NUM_ARGS 0

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

	// TODO: Listen on this socket.
    if (listen(sock, MAX_CONNECTIONS) == -1) {
        perror("Unable to listen on server socket. Aborting...");
        exit(EXIT_FAILURE);
    }

	// A server typically runs infinitely, with some boolean flag to terminate.
	while (1) {

		// Now accept the incoming connections.
		struct sockaddr_in clientAddress;

		socklen_t size = sizeof(struct sockaddr_in);

		// TODO: Accept a connection.
        int recvFd;
		if ((recvFd = accept(sock, (struct sockaddr *)&clientAddress, &size)) == -1) {
            perror("Unable to accept on server. Aborting...");
            exit(EXIT_FAILURE);
        }

		// Buffer for data.
		char buffer[256];
        memset(buffer, 0, 256);
        int read_size = -1;

        printf("Accepted connection!Read in ");
		// TODO: Read from the socket and print the contents.
        while ((read_size = read(recvFd, buffer, 255)) > 0) {
            printf("%s", buffer);
            fflush(stdout);
            memset(buffer, 0, 256);
        }
        if (read_size == -1) {
            perror("Unable to read on server. Aborting...");
            exit(EXIT_FAILURE);
        }

		// TODO: Close the connection.
        close(recvFd);
	}

	// Close the server socket.
	close(sock);
}
