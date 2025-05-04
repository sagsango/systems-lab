/*
 *
 * We dont need to use any kind of syncronization machanish for the
 * write_pointer & read_pointer.
 *
 * But Written data may not be visible to the reader. So we
 * will need some memory barriers (from memory model)
 *
 *
 * TODO: Read baout memory models (in pthread_lock LOCK prefix automatically put
 * the barrrier by the cpu
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#ifndef ROUNDS
#define ROUNDS 500
#endif

#ifndef SIZE
#define SIZE 1000
#endif

/* TODO: Enable logs here!!! */
#define CAPACITY (SIZE)

//#define DEBUG

#ifdef DEBUG
#define log(x,y) printf(x, y)
#else
#define log(x,y)
#endif

/* XXX: We can not have more than 2 threads :) */
#define N_THREAD_ENQUE 1
#define N_THREAD_DEQUE 1

typedef int Item; // Change this to the type of items you want to store

/*
 * XXX
 * One data item size of memory getting wasted
 * We can avoid that by having size, but it will create data races
 *
 */
typedef struct {
	volatile int head;
	volatile int tail;
	Item items[SIZE];
} TwoThreadedQueue;

TwoThreadedQueue queue;

void TwoThreadedQueue_init(TwoThreadedQueue *queue) {
	queue->head = 0;
	queue->tail = 0;
}

void TwoThreadedQueue_enq(TwoThreadedQueue *queue, Item x) {
	// Busy wait while queue is full
	while ((queue->tail + 1) % SIZE == queue->head);

	queue->items[queue->tail % SIZE] = x;
	queue->tail++;

	log("Enqueued: %d\n", x);
}

Item TwoThreadedQueue_deq(TwoThreadedQueue *queue) {
	// Busy wait while queue is empty
	while (queue->head == queue->tail);

	Item item = queue->items[queue->head % SIZE];
	queue->head++;

	log("Dequeued: %d\n", item);

	return item;
}

void* enq(void* arg) {
	for (int r=0; r<ROUNDS; ++r) {
		for (int i = 0; i < SIZE; i++) {
			Item item = i + 1; // Example: items are 1, 2, 3, ...

			TwoThreadedQueue_enq(&queue, item);
		}
	}
	return NULL;
}

void* deq(void* arg) {
	for (int r=0; r<ROUNDS; ++r) {
		for (int i = 0; i < SIZE; i++) {
			Item item = TwoThreadedQueue_deq(&queue);
			assert (item == i+1);
		}
	}
	return NULL;
}

int main() {
	pthread_t enq_thread[N_THREAD_ENQUE], deq_thread[N_THREAD_DEQUE];

	// Initialize queue
	TwoThreadedQueue_init(&queue);

	// Create threads
	for (int i=0; i<N_THREAD_ENQUE; ++i) 
		pthread_create(&enq_thread[i], NULL, enq, NULL);
	for (int i=0; i<N_THREAD_DEQUE; ++i) 
		pthread_create(&deq_thread[i], NULL, deq, NULL);

	// Join threads
	for (int i=0; i<N_THREAD_ENQUE; ++i)
		pthread_join(enq_thread[i], NULL);
	for (int i=0; i<N_THREAD_DEQUE; ++i)
		pthread_join(deq_thread[i], NULL);

	return 0;
}
