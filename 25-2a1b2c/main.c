#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define N 1000

pthread_t t[5*N];

#define sync __SYNC_ALL_LTHREAD_FIRST_
pthread_barrier_t sync;
pthread_barrier_t abc;

sem_t a, b, c;

void *
workerA(void * arg) {
    char id = (char) arg;
    pthread_barrier_wait(&sync); // Let eveyone start from here

    sem_wait(&a);
    write(1, &id, 1);
    pthread_barrier_wait(&abc);
		sem_post(&a);
}


void *
workerB(void * arg) {
    char id = (char) arg;
    pthread_barrier_wait(&sync); // Let eveyone start from here

    sem_wait(&b);
    write(1, &id, 1);
    pthread_barrier_wait(&abc);
		sem_post(&b);
}


void *
workerC(void * arg) {
    char id = (char) arg;
    pthread_barrier_wait(&sync); // Let eveyone start from here

    sem_wait(&c);
    write(1, &id, 1);
    pthread_barrier_wait(&abc);
		sem_post(&c);
}

int main() {
    pthread_barrier_init(&sync, 0, 5*N);

    pthread_barrier_init(&abc, 0, 5);

    sem_init(&a, 0, 2);
    sem_init(&b, 0, 1);
    sem_init(&c, 0, 2);

    for (int i=N*0; i<N*2; ++i) {
        pthread_create(&t[i], 0, workerA, (void*)'A');
    }
    for (int i=N*2; i<N*3; ++i) {
        pthread_create(&t[i], 0, workerB, (void*)'B');
    }
    for (int i=N*3; i<N*5; ++i) {
        pthread_create(&t[i], 0, workerC, (void*)'C');
    }
    for (int i=0; i<5*N; ++i) {
        pthread_join(t[i], NULL);
    }
}
