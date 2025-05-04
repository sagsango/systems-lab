/*
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
	atomic_store_explicit(&rb->head, next_head, memory_order_release);
	return true;
}

bool rb_consume(RingBuffer* rb, uint8_t* value) {
	uint32_t current_tail = rb->tail;

	// Check if buffer is empty
	if (current_tail == rb->head) {
		return false;
	}

	*value = rb->data[current_tail];
	atomic_store_explicit(&rb->tail, 
			      (current_tail + 1) & (BUFFER_SIZE - 1), 
			      memory_order_release);
	return true;
}
