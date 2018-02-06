#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv) {

	if (argc < 2) {

		printf("Incorrect number of args: given %d, expected 1.\n", argc - 1);
		exit(1);
	}

	const char *n = argv[1];
	printf("Inputted number is %s\n", n);
	printf("Parent running with PID  = %d\n", getpid());

	pid_t childPid1 = fork();
	if (childPid1 == 0)
	{
			char * args[10];
			args[0] = "/home/vijay094/ws/OS/discussion3/rtime.o";
			args[1] = n;
			args[2] = (char *)NULL;
			execv("/home/vijay094/ws/OS/discussion3/rtime.o", args);
	}
	else
	{
			printf("Child forked with PID = %d for execv()\n", childPid1);
			waitpid(childPid1, 0, 0);
			printf("execv completed.\n");

			pid_t childPid2 = fork();
			if (childPid2 == 0)
			{
					execl("/home/vijay094/ws/OS/discussion3/rtime.o", "rtime.o", n, (char *)NULL);
			}
			else
			{
					printf("Child forked with PID = %d execl()\n", childPid2);
					waitpid(childPid2, 0, 0);
					printf("execl completed.\n");
			}
	}
}
