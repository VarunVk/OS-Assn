#define _BSD_SOURCE
#define NUM_ARGS 3

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

void shallowCopy(char* old, char* new) {

	// TODO: Create links to all files in old in new.

}

void deepCopy(char* old, char* new) {

	// TODO: Copy the contents of all files in old to new.

}

int main(int argc, char** argv) {

	if (argc < NUM_ARGS + 1) {

		printf("Wrong number of args, expected %d, given %d\n", NUM_ARGS, argc - 1);
		exit(1);
	}

	// TODO: Time how long it takes to execute each copy and print the results.

}
