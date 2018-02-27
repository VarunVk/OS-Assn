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

    DIR *dir;
    struct dirent *dp;
    struct stat Fstat;

    // If create the new folder if does not exist
    if(!(stat(new, &Fstat) == 0 && S_ISDIR(Fstat.st_mode)))
        mkdir(new, 0700);

    if ((dir = opendir(old)) == NULL) {
        printf("Directory %s does not exist\n", old);
        return;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") || (dp->d_type == DT_DIR))
            continue;

        char dst_path[256], src_path[256];
        sprintf(dst_path, "%s/%s", new, dp->d_name);
        sprintf(src_path, "%s/%s", old, dp->d_name);

        if (link(src_path, dst_path) == -1) {
            printf("Could not create the hard copy for %s. Errno %d\n",
                   src_path, errno);
        }
    }
}

void deepCopy(char* old, char* new) {

    DIR *dir;
    struct dirent *dp;
    struct stat Fstat;

    // If create the new folder if does not exist
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
        sprintf(src_path, "%s/%s", old, dp->d_name);
        sprintf(dst_path, "%s/%s", new, dp->d_name);

        int size = 0;
        struct stat src_stat;
        lstat(src_path, &src_stat);

        if (S_ISLNK(src_stat.st_mode))
        {
            int src_size = (int) src_stat.st_size;
            char *buf = malloc(src_size+1);
            memset(buf, 0, src_size+1);
            if ((size = readlink(src_path, buf, src_size)) > 0)
            {
                buf[size] = '\0';
                if (symlink(buf, dst_path) != 0) {
                    printf("There was a problem creating a link %s --> %s (Errno %d)\n",
                           dst_path, buf, errno);
                }
            }
            if (buf != NULL)
                free(buf);
        } else {
            char *buf = malloc(256);
            memset(buf, 0, 256);
            int src_fd;
            if ((src_fd = open(src_path, O_RDONLY)) < 0)
            {
                printf("Skipping %s as it couldn't be opened. Errno %d\n", src_path, errno);
                continue;
            }
            int dst_fd;
            if ((dst_fd = open(dst_path, O_WRONLY | O_CREAT)) < 0)
            {
                printf("Skipping %s as it couldn't be opened. Errno %d\n", dst_path, errno);
                continue;
            }

            while ((size = read(src_fd, buf, 256)) > 0)
            {
                int nwrite = 0;
                if ((nwrite = write(dst_fd, buf, size)) != size) {
                    printf("There was a problem while copying data from %s to %s. Read %d but wrote %d Errno %d\n",
                           src_path, dst_path, size, nwrite, errno);
                }
                memset(buf, 0, 256);
            }
            close(src_fd);
            close(dst_fd);
            if (buf != NULL)
                free(buf);
        }
        chmod(dst_path, src_stat.st_mode);
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
    double deepTime    = (double) deepTimeVal.tv_sec    + 1.e-6 * (double)deepTimeVal.tv_usec;
    double shallowTime = (double) shallowTimeVal.tv_sec + 1.e-6 * (double)shallowTimeVal.tv_usec;

    printf("Shallow Copy    = %.4f second\n", shallowTime - start);
    printf("Deep Copy       = %.4f second\n", deepTime - shallowTime);

}
