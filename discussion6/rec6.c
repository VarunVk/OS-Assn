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
#include <sys/time.h>
#include <errno.h>

void shallowCopy(char* old, char* new) {

	// TODO: Create links to all files in old in new.
	DIR *dir;
	struct dirent *dp;
    struct stat Fstat;

    if(!(stat(new, &Fstat) == 0 && S_ISDIR(Fstat.st_mode)))
        mkdir(new, 0700);

	if ((dir = opendir(old)) == NULL) {
        printf("Directory does not exist: %s\n", old);
        return;
    }

    while ((dp = readdir(dir)) != NULL)
    {
		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") || (dp->d_type == DT_DIR))
			continue;

		char dst_path[256], src_path[256];
        sprintf(dst_path, "%s/%s", new, dp->d_name);
        sprintf(src_path, "%s/%s", old, dp->d_name);
		printf("Copying %s ---> %s\n", src_path, dst_path);
		if (link(src_path, dst_path) == -1) {
			printf("Could not copy. Errno %d\n", errno);
		}
	}
}

void deepCopy(char* old, char* new) {

	// TODO: Copy the contents of all files in old to new.
	DIR *dir;
	struct dirent *dp;
    struct stat Fstat;

    if(!(stat(new, &Fstat) == 0 && S_ISDIR(Fstat.st_mode)))
        mkdir(new, 0700);

	if ((dir = opendir(old)) == NULL) {
        printf("Directory does not exist: %s\n", old);
        return;
    }

    while ((dp = readdir(dir)) != NULL)
    {
		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") || (dp->d_type == DT_DIR))
			continue;

		char dst_path[256], src_path[256];
		struct stat src_stat;
		lstat(src_path, &src_stat);
		int src_size = (int) src_stat.st_size;
		sprintf(src_path, "%s/%s", old, dp->d_name);
        sprintf(dst_path, "%s/%s", new, dp->d_name);
		printf("Copying %s (=%d) ---> %s\n", src_path, src_size, dst_path);

		int src_fd = open(src_path, O_RDONLY);
		int dst_fd = open(dst_path, O_WRONLY | O_CREAT);

		int size = 0;
		if (S_ISLNK(src_stat.st_mode))
		{
            char *buf = malloc(src_size+1);
            memset(buf, 0, src_size+1);
			if ((size = readlink(src_path, buf, src_stat.st_size)) > 0)
			{
				buf[size] = '\0';
				if (write(dst_fd, buf, size) != size) {
					printf("There was a problem while copying data from %s to %s\n", src_path, dst_path);
				}
			}
            if (buf != NULL)
                free(buf);
		} else {
            char *buf = malloc(256);
            memset(buf, 0, 256);
			while ((size = read(src_fd, buf, 256)) > 0)
			{
                int nwrite = 0;
				if ((nwrite = write(dst_fd, buf, size)) != size) {
					printf("1 There was a problem while copying data from %s to\
                           %s. Read %d but wrote %d Errno %d\n", src_path, dst_path,
                           size, nwrite, errno);
				}
				memset(buf, 0, 256);
			}
            if (buf != NULL)
                free(buf);
		}
		close(src_fd);
		close(dst_fd);
	}
}

int main(int argc, char** argv) {

	if (argc < NUM_ARGS + 1) {

		printf("Wrong number of args, expected %d, given %d\n", NUM_ARGS, argc - 1);
		exit(1);
	}

    struct timeval startTimeVal, shallowTimeVal, deepTimeVal;
    gettimeofday(&startTimeVal, NULL);

	shallowCopy(argv[1], argv[2]);
    gettimeofday(&shallowTimeVal, NULL);

	deepCopy(argv[1], argv[3]);
    gettimeofday(&deepTimeVal, NULL);

    double start       = (double) startTimeVal.tv_sec   + 1.e-6 * (double)startTimeVal.tv_usec;
    double deepTime    = (double) deepTimeVal.tv_sec    + 1.e-6 *  (double)deepTimeVal.tv_usec;
    double shallowTime = (double) shallowTimeVal.tv_sec + 1.e-6 * (double)shallowTimeVal.tv_usec;
    printf("Shallow Copy    = %.4f\n", shallowTime - start);
    printf("Deep Copy       = %.4f\n", deepTime - shallowTime);
	// TODO: Time how long it takes to execute each copy and print the results.

}
