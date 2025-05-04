#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define BUFFER_CAPACITY 10000
#define P_N 16
#define C_N 16
#define P_ITER 100000
#define C_ITER 100000

typedef struct {
	int buffer[BUFFER_CAPACITY];
	size_t head;
	size_t tail;
	_Atomic size_t size;
	pthread_mutex_t enqueue_lock;  /* CAS on producer end */
	pthread_mutex_t dequeue_lock;  /* CAS on consumer end */
} RingBuffer;

RingBuffer* rb_init() {
	RingBuffer* rb = malloc(sizeof(RingBuffer));
	if (!rb) return NULL;

	rb->head = 0;
	rb->tail = 0;
	atomic_init(&rb->size, 0);

	pthread_mutex_init(&rb->enqueue_lock, NULL);
	pthread_mutex_init(&rb->dequeue_lock, NULL);

	return rb;
}

void rb_destroy(RingBuffer* rb) {
	if (rb) {
		pthread_mutex_destroy(&rb->enqueue_lock);
		pthread_mutex_destroy(&rb->dequeue_lock);
		free(rb);
	}
}

bool rb_enqueue(RingBuffer* rb, int value) {
	pthread_mutex_lock(&rb->enqueue_lock);

	if (atomic_load(&rb->size) >= BUFFER_CAPACITY) {
		pthread_mutex_unlock(&rb->enqueue_lock);
		return false;
	}

	rb->buffer[rb->tail] = value;
	rb->tail = (rb->tail + 1) % BUFFER_CAPACITY;

	// Atomically increment size
	atomic_fetch_add(&rb->size, 1);

	pthread_mutex_unlock(&rb->enqueue_lock);
//	printf("1");
	return true;
}

bool rb_dequeue(RingBuffer* rb, int* value) {
	pthread_mutex_lock(&rb->dequeue_lock);

	if (atomic_load(&rb->size) == 0) {
		pthread_mutex_unlock(&rb->dequeue_lock);
		return false; // Buffer empty
	}

	*value = rb->buffer[rb->head];
	rb->head = (rb->head + 1) % BUFFER_CAPACITY;

	// Atomically decrement size
	atomic_fetch_sub(&rb->size, 1);

	pthread_mutex_unlock(&rb->dequeue_lock);
//	printf("0");
	return true;
}


void *f_producer(void *arg) {
	RingBuffer *rb = (RingBuffer*) arg;
	int produced = 0;
	for (int i=0; i<P_ITER; ++i) {
		if (rb_enqueue(rb, i)) {
			++produced;
		}
	}
	return (void*)produced;
}

void *f_consumer(void *arg) {
        RingBuffer *rb = (RingBuffer*) arg;
        int consumed = 0, val;
        for (int i=0; i<C_ITER; ++i) {
                if (rb_dequeue(rb, &val)) {
                        ++consumed;
                }
        }
        return (void*)consumed;
}

int test() {
	RingBuffer* rb = rb_init();
	pthread_t p[P_N], c[C_N];

	if (!rb) {
		printf("Failed to initialize ring buffer\n");
		return 1;
	}

	for (int i=0; i<P_N; ++i) {
		pthread_create(&p[i], NULL, f_producer, rb);
	}
	for (int i=0; i<C_N; ++i) {
		pthread_create(&c[i], NULL, f_consumer, rb);
	}

	int rem = 0, add = 0;
	for (int i=0; i<P_N; ++i) {
		int status;
		pthread_join(p[i], (void**)&status);
		add += status;
	}
	for (int i=0; i<C_N; ++i) {
		int status;
		pthread_join(c[i], (void**)&status);
		rem += status;
	}
	printf("Add: %d, Rem: %d, Size:%d\n", add, rem, atomic_load(&rb->size));
	assert (add == rem + atomic_load(&rb->size));
	rb_destroy(rb);
	return 0;
}
int main() {
	// TODO: Write test
	//	1. one reader & one writer
	//	2. multiple reader & multiple writer
	test();
}
