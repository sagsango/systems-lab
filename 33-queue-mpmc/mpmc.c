/*
 * May 12, 2025:
 * 		1. we can do DCAS on (read,write) for producer and
 * 		also consumer!!
 * 		2. CAS on read or write end because other end will not affect another, 
 * 		   And CAS will make sure that each end only guy wins!
 * 
 * There is a write-write conflict in producers side(many producer will fight)
 * There is a write-write conflict in consumers size(many consumer will fight)
 *
 * But between producers and consumers threre should be only visibility.
 * Means write should be visible to other size (Write - Read conflict)
 *
 *
 * As there will be CAS in reader side which will take lock on cacheline
 * As there will be CAS on writer side which will take lock on cacheline
 * So LOCK prefix will automatically put barrier (done by CPU)
 *
 *
 * (NOTE: If we take shared size then there will be contension between
 * noth sides, the producers and the consumers)
 *
 */
#include <stdint.h>
#include <stdatomic.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024 // Must be power of 2

typedef struct {
	uint8_t data[BUFFER_SIZE];
	_Atomic uint32_t head;
	_Atomic uint32_t tail;
} RingBuffer;

void rb_init(RingBuffer* rb) {
	rb->head = 0;
	rb->tail = 0;
}

bool rb_produce(RingBuffer* rb, uint8_t value) {
	uint32_t current_head = rb->head;
	uint32_t next_head = (current_head + 1) & (BUFFER_SIZE - 1);

	// Check if buffer is full
	if (next_head == rb->tail) {
		return false;
	}

	rb->data[current_head] = value;
	atomic_store_explicit(&rb->head, next_head, memory_order_release); /* TODO: As there are multiple producers it should be CAS */
	/* And also the increment in head & data written at head should be atomic; otherwise it will not sync with the cosumer *
	 * Problem A: If we write data before incrementing the head -> Data can be overwritten by another producer 
	 * Problem B: If we increment the head then write data -> Consumer can read the invalid data meanwhile
	 */
	return true;
}

bool rb_consume(RingBuffer* rb, uint8_t* value) {
	uint32_t current_tail = rb->tail;

	// Check if buffer is empty
	if (current_tail == rb->head) {
		return false;
	}
	// TODO: Here it should be memory accuire before rading the value!!
	*value = rb->data[current_tail];
	atomic_store_explicit(&rb->tail, 
			      (current_tail + 1) & (BUFFER_SIZE - 1), 
			      memory_order_release); /* TODO: Here it should be relaxed!!!
	return true;
}
