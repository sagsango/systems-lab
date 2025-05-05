// TODO:
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <wait.h>

#define MAP_SIZE (1<<10)

int main() {
    void * addr = mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (addr == MAP_FAILED) {
        perror("mapped failed");
    }

    long * id = (long*)addr;
    *id = 1;

    switch (fork()) {
        case -1: {
            perror("fork failed");
            break;
        }
        case 0: {
            printf("Child reads: %ld\n", *id);
            *id = 2;
            break;
        }
        default: {
            wait(NULL);
            printf("Parent reads: %ld\n", *id);
            break;
        }
    }
}