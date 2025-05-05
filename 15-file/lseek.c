//TODO
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/fcntl.h>
#include <string.h>
#include <memory.h>

#define FILENAME "testfile.txt"

int main() {
    int status, off, n;
    int fd = open(FILENAME, O_CREAT | O_EXCL | O_RDWR);
    if (fd < 0) {
        perror("Cant open/create file");
    }
    char buf[128];

    /* Write*/
    memset(buf, 0, 128);
    memset(buf, '1', 10);
    n = 10;
    while (n) {
        status = write(fd, buf, n);
        if (status < 0) {
            perror("Write failed");
            break;
        }
        n -= status;
    }
    printf("Written: %s\n", buf);



    /* lseek*/
    off = lseek(fd, 0, SEEK_SET);
    printf ("offset: %d\n", off);


    /*read*/
    memset(buf, 0, 128);
    status = read(fd, buf, 128);
    if (status < 0) {
        perror("Read failed");
    }
    printf("Did Read: %s\n", buf);


    /*lseek*/
    off = lseek(fd, 0, SEEK_SET);
    printf ("offset: %d\n", off);


    /* write*/
    memset(buf, 0, 128);
    memset(buf, '2', 5);
    n = 5;
    while (n) {
        status = write(fd, buf, n);
        if (status < 0) {
            perror("Write failed");
            break;
        }
        n -= status;
    }
    printf("Written: %s\n", buf);


    /* lseek */
    off = lseek(fd, 0, SEEK_SET);
    printf ("offset: %d\n", off);


    /*Read*/
    memset(buf, 0, 128);
    status = read(fd, buf, 128);
    if (status < 0) {
        perror("Read failed");
    }
    printf("Did Read: %s\n", buf);

    /*close*/
    status = close(fd);
    if (status < 0) {
        perror("Fail to close file");
    }


    /*unlink*/
    status = unlink(FILENAME);
    if (status < 0) {
        perror("can remove file");
    }

}
