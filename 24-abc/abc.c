#include <stdio.h>
#include <pthread.h>


#define N 100

pthread_t t[3*N];

pthread_barrier_t sync;
pthread_barrier_t ab;
pthread_barrier_t bc;

sem_t a, b, c;

void *
workerA(void * arg) {
    char id = (char) arg;
    pthread_barrier_wait(&sync); // Let eveyone start from here

    sem_wait(&a);
    write(1, &id, 1);
    pthread_barrier_wait(&ab);
}


void *
workerB(void * arg) {
    char id = (char) arg;
    pthread_barrier_wait(&sync); // Let eveyone start from here

  
    sem_wait(&b);
    pthread_barrier_wait(&ab);
    write(1, &id, 1);
    pthread_barrier_wait(&bc);
}


void *
workerC(void * arg) {
    char id = (char) arg;
    pthread_barrier_wait(&sync); // Let eveyone start from here

    sem_wait(&c);
    pthread_barrier_wait(&bc);
    write(1, &id, 1);
    sem_post(&a);
    sem_post(&b);
    sem_post(&c);
}

int main() {
    pthread_barrier_init(&sync, 0, 3*N);

    pthread_barrier_init(&ab, 0, 2);
    pthread_barrier_init(&bc, 0, 2);

    sem_init(&a, 0, 1);
    sem_init(&b, 0, 1);
    sem_init(&c, 0, 1);

    for (int i=N*0; i<N*1; ++i) {
        pthread_create(&t[i], 0, workerA, (void*)'A');
    }
    for (int i=N*1; i<N*2; ++i) {
        pthread_create(&t[i], 0, workerB, (void*)'B');
    }
    for (int i=N*2; i<N*3; ++i) {
        pthread_create(&t[i], 0, workerC, (void*)'C');
    }
    for (int i=0; i<3*N; ++i) {
        pthread_join(t[i], NULL);
    }
}