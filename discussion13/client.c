#define _BSD_SOURCE
#define NUM_ARGS 3

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

int main(int argc, char** argv) {

	if (argc < NUM_ARGS + 1) {

		printf("Wrong number of args, expected %d, given %d\n", NUM_ARGS, argc - 1);
		exit(1);
	}

	// Create a TCP socket.
	int sock = socket(AF_INET , SOCK_STREAM , 0);

	// Specify an address to connect to (we use the local host or 'loop-back' address).
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(SERVER_PORT);
	address.sin_addr.s_addr = inet_addr(argv[1]);

	// Connect it.
	if (connect(sock, (struct sockaddr *) &address, sizeof(address)) == 0) {

		// TODO: Write data to socket.
		char Message[1024];
		sprintf(Message, "%s %s", argv[2], argv[3]);
		if (send(sock, Message, strlen(Message), 0) == -1) {
			perror("Unable to send data to server. Errno: ");
		}

		// TODO: Read the data sent back.
		char recvBuf[1024] = {0};
		ssize_t recvSize = -1;
		if ((recvSize = read(sock, recvBuf, 1024)) == -1) {
			perror("Error while receiving response from server. Errno: ");
		}
		recvBuf[recvSize] = '\0';
		printf("Recvd %s\n", recvBuf);

		// Close the socket.
		close(sock);

	} else {

		perror("Connection failed!");
	}
}
