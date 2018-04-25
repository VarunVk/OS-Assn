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
#include <semaphore.h>

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
struct bankAccount *Accounts;
sem_t sem;

void *threadFunction(void* args) {

	struct threadArg* arg = args;

	// TODO: Alter the specified account;
	struct bankAccount *Ac = Accounts + arg->accountNumber;
	int balance;
	pthread_mutex_lock(Ac->mutex);
	Ac->balance += arg->change;
	balance = Ac->balance;
	pthread_mutex_unlock(Ac->mutex);

	// TODO: Write back to client.
	char Message[1024];
	sprintf(Message, "%d", balance);
	if (send(arg->clientfd, Message, strlen(Message), 0) == -1) {
		perror("Unable to send data to server. Errno: ");
	}
	// Close client connection.
	close(arg->clientfd);
	sem_post(&sem);

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
	const int numAccounts = atoi(argv[1]);
	printf("numAccounts =%d\n", numAccounts);
	Accounts = calloc(numAccounts, sizeof(struct bankAccount));
	struct bankAccount *temp = Accounts;
	for (int i=0; i<numAccounts; i++)
	{
		temp->mutex = malloc(sizeof(pthread_mutex_t));
		if (pthread_mutex_init(temp->mutex, NULL)) {
			perror("Error: Failed to initialize mutex lock: ");
			exit(EXIT_FAILURE);
		}
		temp++;
	}

	// TODO: Init threads.
	int Max_threads = atoi(argv[2]);
	pthread_t tid[Max_threads];
	int index = 0;
	sem_init(&sem, 0, Max_threads);

	// A server typically runs infinitely, with some boolean flag to terminate.
	while (1) {

		sem_wait(&sem);
		// Now accept the incoming connections.
		struct sockaddr_in clientAddress;

		socklen_t size = sizeof(struct sockaddr_in);
		int clientfd = accept(sock, (struct sockaddr*) &clientAddress, &size);

		// TODO: Read request from the socket.
        unsigned int recvSize = -1;
		char recvBuf[1024];
		if ((recvSize = recv(clientfd, recvBuf, 1024, 0)) > 0) {
            recvBuf[recvSize] = '\0';
            printf("Request received %s\n", recvBuf);
        }

		// TODO: Run a thread to handle the request.
		char *savePtr;
		struct threadArg *args = calloc(1, sizeof(struct threadArg));
		args->accountNumber = atoi(strtok_r(recvBuf, " ", &savePtr));
		args->change        = atoi(strtok_r(NULL, " ", &savePtr));
		args->clientfd      = clientfd;
		printf("Parsed data Ac Num %d change %f\n", args->accountNumber, args->change);

		pthread_create(&tid[index%Max_threads], NULL, threadFunction, args);
		index++;
	}

	// Close the socket.
	close(sock);
}
