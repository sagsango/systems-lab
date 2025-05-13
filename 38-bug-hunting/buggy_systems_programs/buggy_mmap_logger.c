#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define FILESIZE 4096

int main() {
    int fd = open("logfile.txt", O_RDWR | O_CREAT, 0666); // 🔴 No error check
    ftruncate(fd, FILESIZE); // 🔴 No error check

    char *map = mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) return 1;

    for (int i = 0; i < 100; i++) {
        sprintf(map + i * 40, "Log entry %d\n", i); // 🔴 May overflow
        msync(map, FILESIZE, MS_SYNC); // 🔴 Inefficient to sync every time
        usleep(10000);
    }

    munmap(map, FILESIZE);
    close(fd);
    return 0;
}