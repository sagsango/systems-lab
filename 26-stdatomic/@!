#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <ctype.h>
#include <pthread.h>


#define N_LOOP 10000000LL
#define N_THREADS 10LL


/*
 * TODO: thread safe linked List
 */

struct node {
	_Atomic
_Atomic long cntr = 0;
_Atomic long fail = 0;

int incr() {
	long long read, next;
	do {
		read = atomic_load(&cntr);
		next = read  + 1;
		atomic_fetch_add(&fail, 1);
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
	atomic_store(&cntr, 0);
	atomic_store(&fail, 0);

	pthread_t t[N_THREADS];

	for (int i=0; i<N_THREADS; ++i) {
			pthread_create(&t[i], 0, tf, 0);
	}
	for (int i=0; i<N_THREADS; ++i) {
		pthread_join(t[i], NULL);
	}
	printf("threads = %lld, loops: %lld, cntr : %lld, failed:%lld\n", N_THREADS, N_LOOP, atomic_load(&cntr), atomic_load(&fail)- (N_LOOP * N_THREADS));
}
