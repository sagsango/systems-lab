#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

#define N 56 
#define SIZE (1<<12)
#define TIMES (0xffff)

char * buf;

pthread_t t[N];
pthread_mutex_t mtx;

void * thread_fun(void * arg) {
  unsigned long id = (unsigned long) arg;

  for (unsigned long i=0; i<TIMES; ++i) {
    pthread_mutex_lock(&mtx);
    int * id_ptr = (int*)(&buf[id * 64]);
    int * cnt_ptr = (int*)(&buf[id * 64 + 32]);
    *id_ptr = id;
    *cnt_ptr = i;
    pthread_mutex_unlock(&mtx);
  }
  return NULL;
}

int main(int argc, char * argv[]) {
  int status;
  int fd = open(argv[1], O_CREAT| O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO );
  if (fd < 0) {
    perror(argv[1]);
    exit(1);
  }

  status = ftruncate(fd, SIZE);
  if (status) {
    exit(1);
  }

  buf = mmap(NULL, SIZE, PROT_WRITE|PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (buf == MAP_FAILED) {
    close(fd); /* Gracefull exit */
    exit(2);
  }
  close(fd);

  status = pthread_mutex_init(&mtx, NULL);
  if (status) {
    exit(3);
  }

  for (unsigned long i=0; i<N; ++i) {
    pthread_create(&t[i], NULL, (void *(*)(void*)) thread_fun, (void *) i);
  }

  for (int i=0; i<N; ++i) {
    pthread_join(t[i], NULL);
  }

  if (msync(buf, SIZE, MS_SYNC)) {
    exit(5);
  }

  if (munmap(buf, SIZE)) {
    exit(1);
  }

  status = pthread_mutex_destroy(&mtx);
  if (status) {
    exit(1);
  }
}
