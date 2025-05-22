#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <ctype.h>
#include <pthread.h>



/*
 * XXX:
 * https://cplusplus.com/reference/atomic/atomic_compare_exchange_strong_explicit/
 * bool atomic_compare_exchange_strong_explicit (volatile A* obj, T* expected, T val, memory_order success, memory_order failure)
*/

#define N_LOOP 10000000
#define N_THREADS 10

_Atomic int cntr = 0;

int incr() {
	int read, next;
	do {
		read = atomic_load(&cntr);
		next = read  + 1;
	}while (!atomic_compare_exchange_strong(&cntr, &read, next));
	return 0;
}

void * tf(void* arg) {
		for (int i=0; i<N_LOOP; ++i) {
				incr();
		}
		return NULL;
}

int main() {
	pthread_t t[N_THREADS];
	for (int i=0; i<N_THREADS; ++i) {
			pthread_create(&t[i], 0, tf, 0);
	}
	for (int i=0; i<N_THREADS; ++i) {
		pthread_join(t[i], NULL);
	}
	printf("threads = %d, loops: %d, cntr : %d\n", N_THREADS, N_LOOP, atomic_load(&cntr));
}
