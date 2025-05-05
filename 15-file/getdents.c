#include <dirent.h>
#include <stdio.h>
#include <sys/file.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/dir.h>
#include <sys/syscall.h>
#include <syscall.h>
#include <dirent.h>


#define FILENAME "/home/ssing214/systems-lab/15-file"
#define BUF_SIZE (1<<10) 
int main() {

    int fd = open(FILENAME, O_RDONLY | __O_DIRECTORY);
    if (fd < 0) {
        perror("Can not open dir");
    }

    char buffer[BUF_SIZE];
    // getdents64(fd, buffer, BUF_SIZE);
    int nbytes;
    while ( (nbytes = syscall(SYS_getdents64, fd, buffer, BUF_SIZE)) > 0) {
            struct dirent *d;
            char * ptr = buffer;
            while (ptr < buffer + nbytes) {
                d = (struct dirent *) ptr;
                printf("%ld, %s, %ld %s\n", d->d_off, d->d_type == DT_REG ? "file" : d->d_type == DT_DIR ? "DIR" : "?" , d->d_ino, d->d_name);
                ptr += d->d_reclen;
            }       
    }

    if (close(fd) < 0) {
        perror("Cant close the dir");
    }
}