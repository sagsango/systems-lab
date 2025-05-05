//TODO

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/dir.h>
#include <sys/vfs.h>
#include <syscall.h>
#include <sys/stat.h>

#define DIRNAME "TestDir"
int main() {
    int status = mkdir(DIRNAME, 0);
    if (status < 0) {
        perror("cant create dir");
    }
    
    status = rmdir(DIRNAME);
    if (status < 0) {
        perror("Cant rmdir");
    }
}