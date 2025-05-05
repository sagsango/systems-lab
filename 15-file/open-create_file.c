//TODO:


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/fcntl.h>
#include <sys/stat.h>

#define FILENAME "testfile.txt"
#define DIRNAME "TestDir"
int main() {
    int status, fd;
    fd = open(FILENAME, O_CREAT | O_RDWR  | O_EXCL, 0644);
    if (fd < 0) {
        perror("Cant create file");
    }

    status = close(fd);
    if (status < 0) {
        perror("Cant close file.");
    }

    status = unlink(FILENAME);



}