// TODO



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/fcntl.h>
#include <pthread.h>

#define FILENAME "testfile.txt"

void * tf(void *arg) {
    int fd = *(int*)arg;

    /*
        TODO: Why no error!!
    */
    int status = flock(fd, LOCK_EX);
    if (status < 0) {
        perror("F_LOCK failed");
    }
    pthread_exit(NULL);
}

int main() {
    int fd, status;

    fd = open(FILENAME, O_CREAT | O_EXCL| O_RDWR, 0644);
    if (fd < 0) {
        perror("Cant create file");
    }

    /* This will give you error !!!*/
    /*
    fd = open(FILENAME, O_CREAT | O_EXCL, 777);
    if (fd < 0) {
        perror("Cant create file");
    }
    */

    status = flock(fd, LOCK_EX);
    if (status < 0) {
        perror("cant lock the file");
    }

    /*
        TODO: Why no error!!
    */
    status = flock(fd, LOCK_EX);
    if (status < 0) {
        perror("cant lock the file");
    }

    pthread_t t;
    pthread_create (&t, 0, tf, &fd);
    pthread_join (t, NULL);


    close(fd);
    status = unlink(FILENAME);
}
