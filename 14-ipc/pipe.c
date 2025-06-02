// TODO:


#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <string.h>
#include <wait.h>


#define R 0
#define W 1

int main() {
    int fd[2];
    int status = pipe(fd);
    if (status < 0) {
        perror("Pipe");
    }
    switch (fork()) {
        case 0: {
            close(fd[R]);
            char * str = "Hello from the other side.\n";
            write (fd[W], str,  strlen(str));
            break;
        }
        case -1: {
            perror("fork() failed\n");
            break;
        }
        default: {
            close(fd[W]);
            char buf[128] = {0};
            read (fd[R], buf, 128);
            printf("Parent received: %s\n", buf);
            wait(NULL);
        }
    }

    /*
    TODO: Use there instead; like a file
    shm_open(const char *name, int oflag, mode_t mode);
    sem_open(const char *name, int oflag, ...);
    mq_open();
*/

}
