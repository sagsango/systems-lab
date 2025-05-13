/*
 * 
 * There are 2 ways: spsc to implement
 * M1: only have read & write pointers
 * 		read will be r by porducer w by consumer 
 *      write will be w by producer r by consumer
 * 			There is no w/w conflict in read or write
 * 
 * 		when producer see the queue is not full,
 * 		(by reading the r & w pointer)
 *      at that time if consumer removes some item
 *      queue still is going to be not full so 
 * 		its safe for the producer to still progrress
 * 		if he saw the queue was not full.
 * 
 * 
 * 		when consumer see queue is not empty
 *      (by reading the r & w pointer) it can 
 *      proceed further because if producer has put
 *      more data meanwhile, it still remains non empty
 *      
 * 
 * 		No cas is needed only visibility (memory-relase and aquire)
 * 		Producer will release 
 * 		Consumer will aquire
 * 
 * 		Downside : We will be having one empty slot always in the queue
 * 		so memory sizeof(data) will be wasted.
 * 
 * TODO: Implement it!!! <<<=================
 * 		But method 2 is slow because we are duing CAS (locking the cacheline)
 * M2: In this senario we will be having size as a member
 * 	   whenre producer and consumer both sync on it.
 * 
 *     as size can be updated by both end at the same time we need
 *     CAS.
 * 
 * 	   lets say producer reads size : s
 * 			now size if not full (s != capacity)
 * 			meanwhile if size get changed by consumer as it will
 * 			decrement then also it would not be full, so same to proceed
 *          now we have to do 2 things:
 * 				write the data at data[w]
 * 				increment the w to w + 1 & size to by 1
 *          
 * 			as there is only one writer so it is safe,
 * 			to first do data[w] = value and then do w = w + 1 & size += 1
 * 
 * 					data[w] = val;
 * 					write = write + 1;
 * 					atmoic_fetch_add_explicit(&size, 1, memory_order_release); // lock the cacheline then increment the value 
 *
 * 
 *		
 * .   similarily for cosumer side:
 *                 if size != 0
 *                    //aquired sementics to get the data from the producer
 *                    val = data[read];
 *                    read = read + 1
 *                 atmoic_fetch_sub_explicit(&size, 1, memory_order_relaxed); // lock the cacheline then increment the value 
 *                 // this time nothing to show to the producer except the size. so relaxed semantic!!! :)
 * 			
 *           
 *            
 * 			
 * 		
 * 							  
 *      
 * 
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
