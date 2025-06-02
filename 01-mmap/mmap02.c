#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sched.h>

struct shared {
	sem_t sem;
	char  *addr;
};

int main(int argc, char*argv[]) {
    int fd = open(argv[1], O_CREAT | O_RDWR);
    if (fd < 0) {
        exit(1);
    }
    if (ftruncate(fd, 1<<15)) {
        exit(2);
    }
    char * addr = mmap(0, 1<<15, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0 );
    if (addr == MAP_FAILED) {
        exit(4);
    }
		struct shared * shared = addr;
    if (sem_init(&shared->sem, 1, 1)) {
        exit(3);
    }
    shared->addr = addr + sizeof(struct shared); 
    int c_pid;
    switch ((c_pid = fork())) {
        case -1: exit(5); break;
        case 0: {
            //child
            for (int i=0; i<100; ++i) {
                sem_wait(&shared->sem);
                for (int i=0; i<26; ++i) {
                    sprintf(shared->addr, "%c", i + 'a');
                    shared->addr += 1;
                }
                sem_post(&shared->sem);
								sched_yield();
            }
						close(fd);
						munmap(addr, 1<<15);
            exit(0);
        }
        default: {
            // parent
            for (int i=0; i<100; ++i) {
                sem_wait(&shared->sem);
                for (int i=0; i<26; ++i) {
                    sprintf(shared->addr, "%c", i + 'A');
                    shared->addr += 1;
                }
                sem_post(&shared->sem);
								sched_yield();  /* For threads we use : pthread_yield(); so that another thread from same thread group will get scheduled */
            }
	            int status;
            wait(&status);
            if (status) {
                exit(6);
            }
						msync(addr, 1<<15, MS_SYNC);
						close(fd);
						munmap(addr, 1<<15);

            exit(0);
        }
    }
}
