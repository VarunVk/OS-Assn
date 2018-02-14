#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char** argv) {

	if (argc < 2) {

		printf("Wrong number of args, expected 1, given %d", argc - 1);
		exit(1);
	}

	// TODO: Your code goes here.
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("Unable to create PIPE. Exiting... ");
        exit(-1);
    }

	int n = atoi(argv[1]);

    pid_t child_pid = fork();
    if (child_pid == 0)
    {
		char* COMMON_FILE = "rec3_output.txt";
 		int COMMON_FD = open(COMMON_FILE, O_WRONLY|O_TRUNC|O_CREAT);
        close(fd[0]);
		chmod(COMMON_FILE, 0700);
		if (dup2(COMMON_FD, STDOUT_FILENO) < 0)
		{
			printf("Unable to dup the STDOUT fd to a common file.\n");
			exit(2);
		}
		write(fd[1], COMMON_FILE, strlen(COMMON_FILE));
		close(fd[1]);

		chmod("rec3.o", 0700);
		chmod("rtime.o", 0700);
		execl("rec3.o", "rec3.o", argv[1], (char*)NULL);
    }
    else
    {
		char COMMON_FILE[1024];
		int status;
        close(fd[1]);
		waitpid(child_pid, &status, 0);
		if (status > 0)
		{
			printf("Child had problems with dup2 Error no %d.\n", WEXITSTATUS(status));
			exit(-1);
		}
		read(fd[0], COMMON_FILE, sizeof(COMMON_FILE));
		close(fd[0]);
		// printf("Output file name is: %s \n", COMMON_FILE);

		char buf[1024];
		char c;
		int COMMON_FD = open(COMMON_FILE, O_RDONLY);
		int numbers_read = 0, prev_value = 0, idx = 0;
		// printf("Numbers read are:\n");
		while (read(COMMON_FD, &c, 1))
		{
			if (c == '\n')
			{
				buf[idx] = '\0';
				int new_value = atoi(buf);
				if (new_value && prev_value < new_value)
				{
					prev_value = new_value;
					// printf("%d \n", new_value);
					memset(buf, 0, sizeof(buf));
					idx = 0;
					numbers_read++;
				}
				else
				{
					if (new_value)
						printf("Values are not in ascending order (New value %d)\n", new_value);
					break;
				}
			}
			else
				buf[idx++] = c;
		}
		if (numbers_read == 2*n)
		{
			printf("PASSED\n");
		}
		else
		{
			printf("FAILED - Read %d numbers expected %d.\n", numbers_read, 2*n);
		}
    }
}
