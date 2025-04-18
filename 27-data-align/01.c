#include <stdio.h>
#include <pthread.h>

#define N_LOOP 1024
#define N_THREAD 64
#define ALIGN 128

struct  __attribute__((aligned(ALIGN))) data {
	pthread_t t;
	pthread_mutex_t mtx;
	int count;
};


struct data tdata[N_THREAD];

void *
tf (void * arg) {
	int i = (int)arg;
	for (int i=0; i<N_LOOP; ++i) {
			pthread_mutex_lock(&tdata[i].mtx);
			tdata[i].count += 1;
			pthread_mutex_unlock(&tdata[i].mtx);
	}
	return NULL;
}

int main() {
	for (int i=0; i<N_THREAD; ++i) {
			pthread_create (&tdata[i].t, 0, tf, (void*)i);
	}
	for (int i=0; i<N_THREAD; ++i) {
			pthread_join(tdata[i].t, NULL);
	}

	return 0;
}

