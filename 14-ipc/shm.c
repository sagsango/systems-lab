// TODO:

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/wait.h>

#define FILENAME "shm.c"
#define SHM_SIZE (1<<10)

  /*
    TODO: Use there instead; like a file
    shm_open(const char *name, int oflag, mode_t mode);
    sem_open(const char *name, int oflag, ...);
    mq_open();
*/
int main() {
    //int fd = shmget(key_t key, size_t size, int shmflg);
    int status;
    int key = ftok(FILENAME, 0);
    if (key < 0) {
        perror("Cant crerate key");
    }
    int id = shmget(key, SHM_SIZE, IPC_CREAT );
    if (id < 0) {
        perror("Cant get shm");
    }

    void * addr = shmat(id, NULL, 0);

    long * data = (long*) addr;
    *data = 0xbadbeef;

    switch (fork()) {
        case -1: {
            perror("Cant do fork");
            break;
        }
        case 0: {
            char buf[32];
          
            printf("Child Read = %p\n", *data);

            *data = 0xbadbeee;
            break;
        }
        default : {
            int status = wait(NULL);
            printf("Parent Read = %p\n", *data);
            break;
        }
    }
}   