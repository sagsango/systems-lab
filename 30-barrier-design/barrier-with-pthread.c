#include <pthread.h>
#include <stdlib.h>

/* XXX:
 *
 * Barrier get resets eveytime,
 * that functionality wqe have to support
 */
struct barrier {
    pthread_cond_t threshold;
    pthread_mutex_t mtx;
    int count, n;
};

struct barrier * new_barrier(int n) {
    struct barrier * b = (struct barrier *) malloc(sizeof(struct barrier));
    if (!b) {
        exit(1);
    }
    pthread_mutex_init(&b->mtx, 0);
    pthread_cond_init(&b->threshold, 0);
}

int barrier_wait(struct barrier * b) {
    pthread_mutex_lock(&b->mtx);
    b->count += 1;
    while (b->count < b->n) {
        pthread_cond_wait(&b->threshold, &b->mtx);
    }
    pthread_cond_broadcast(&b->threshold);
    pthread_mutex_unlock(&b->mtx);
}
