/*
 *
 * This is an old API
 * we should always use open
 */



#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/file.h>

#define FILENAME "textfile.txt"

int main() {

    int status, fd;

    fd = creat("textfile.txt", O_EXCL | O_RDWR); /* O_EXCL is supported with only open()*/
    if (fd < 0) {
        perror("cant create file");
    }

    fd = creat("textfile.txt", O_EXCL | O_RDWR); /* O_EXCL is supported with only open()*/
    if (fd < 0) {
        perror("cant create file");
    }

    status = close(fd);
    if (status < 0) {
        perror("cant close file");
    }

    status = unlink(FILENAME);
    if (status < 0) {
        perror("cant close file");
    }



    


}
