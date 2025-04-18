#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
	int fd = open("mmap2.txt", O_CREAT | O_RDWR);
	if (fd < 0) {
		exit(1);
	}
	if (ftruncate(fd, 1<<10)) {
		exit(2);
	}
	char * addr = mmap(0, 1<<10, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0 );
	if (addr == MAP_FAILED) {
		exit(4);
	}
	/* XXX:WARNING:XXX
	 * This should be present in the shared memory, if we want it to be shared */
	sem_t * sem = addr;
	if (sem_init(sem, 1, 1)) {
		exit(3);
	}
	addr += sizeof(sem_t);
	int c_pid;
	switch ((c_pid = fork())) {
		case -1: exit(5); break;
		case 0: {
							//child
							for (int i=0; i<100; ++i) {
								sem_wait(&sem);
								for (int i=0; i<26; ++i) {
									sprintf(addr, "%c", i + 'a');
									addr += 1;
								}
								sem_post(&sem);

							}
							exit(0);
						}
		default: {
							 // parent
							 for (int i=0; i<100; ++i) {
								 sem_wait(&sem);
								 for (int i=0; i<26; ++i) {
									 sprintf(addr, "%c", i + 'A');
									 addr += 1;
								 }
								 sem_post(&sem);
							 }
							 int status;
							 wait(&status);
							 if (status) {
								 exit(6);
							 }
							 exit(0);
						 }
	}
}
