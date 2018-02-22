#define _BSD_SOURCE

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

struct file_stats {
    char name[256];
    uid_t id;
    float size;
    struct timespec time_lastAccess;
};

void printStats(char* path, const int max) {
    DIR *dir;
    struct dirent *dp;
    struct stat statbuf;
    int total_dir_size = 0;
    int count = 0;

    if ((dir = opendir(path)) == NULL) {
        printf("Directory does not exist: %s\n", path);
        return;
    }

    struct file_stats *Files = malloc(sizeof(struct file_stats) * max);
    while ((dp = readdir(dir)) != NULL && (count < max))
    {
        char d_path[256];
        sprintf(d_path, "%s/%s", path, dp->d_name);

        if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
            continue;

        if(dp->d_type == DT_DIR) {
            printStats(d_path, max);
        } else {
            memset(&statbuf, 0, sizeof(struct stat));
            if (stat(d_path, &statbuf) == 0)
            {
                // Copy the required fields to the struct Files
                strcpy(Files[count].name, dp->d_name);
                Files[count].id              = statbuf.st_uid;
                Files[count].time_lastAccess = statbuf.st_atim;
                Files[count].size            = statbuf.st_size;
                total_dir_size              += statbuf.st_size;
                count++;
            }
            else
            {
                printf("Unable to stat %s\n", dp->d_name);
            }
        }
    }
    // Print all the entries in Files with total folder size
    for (int i=0; i<count; i++)
    {
        struct timespec present_time;
        clock_gettime(CLOCK_REALTIME, &present_time);
        printf("File: %s\n",  Files[i].name);
        printf("\tOwner: %d\n", Files[i].id);
        printf("\tSize: %f\n",  Files[i].size/total_dir_size);
        printf("\tTime since access (sec): %d\n", (int)(present_time.tv_sec - Files[i].time_lastAccess.tv_sec));
    }
    if (Files)
        free(Files);
    closedir(dir);
}

int main(int argc, char** argv) {

	if (argc < 3) {

		printf("Wrong number of args, expected 2, given %d", argc - 1);
		exit(1);
	}

	printStats(argv[1], atoi(argv[2]));
}
