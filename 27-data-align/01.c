#include <stdio.h>
#include <stddef.h>
#include <pthread.h>

#define N_LOOP (10000)
#define N_THREAD 64
#define ALIGN 1024

struct  __attribute__((aligned(ALIGN))) data {
	pthread_t t;
	pthread_mutex_t mtx;
	int count;
};


struct data tdata[N_THREAD];

void *
tf (void * arg) {
	unsigned long id = (unsigned long)arg;
	for (int i=0; i<N_LOOP; ++i) {
			pthread_mutex_lock(&tdata[id].mtx);
			tdata[id].count += 1;
			pthread_mutex_unlock(&tdata[id].mtx);
	}
	return NULL;
}

int main() {
	printf ("%d, %d, %d\n", offsetof(struct data, t), offsetof(struct data, mtx), offsetof(struct data, count));
	for (unsigned long i=0; i<N_THREAD; ++i) {
			pthread_mutex_init (&tdata[i].mtx, NULL);
			pthread_create (&tdata[i].t, 0, tf, (void*)i);
	}
	for (int i=0; i<N_THREAD; ++i) {
			pthread_join(tdata[i].t, NULL);
	}

	return 0;
}

