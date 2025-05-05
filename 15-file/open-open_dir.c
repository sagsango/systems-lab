//TODO:


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/fcntl.h>
#include <sys/stat.h>

#define FILENAME "testfile.txt"
#define DIRNAME "TestDir"
int main() {
    int status, fd;
    status =  mkdir (DIRNAME, 0777);
    if (status < 0) {
        perror("Cant create dir");
    }
    fd = open(DIRNAME, O_RDONLY| __O_DIRECTORY, 0644);
    if (fd < 0) {
        perror("Cant open dir");
    }

    status = close(fd);
    if (status < 0) {
        perror("Cant close file.");
    }

    status = rmdir(DIRNAME);
    if (status < 0) {
        perror("Cant rmdir dir");
    }
}