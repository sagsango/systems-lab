#include <stdio.h>
#include <stdlib.h>
#include "hw4.h"

// Memory region globals
mem_region_t data_mem;
mem_region_t stack_mem;

// Simulated heap boundaries (for testing)
static void* heap_start = NULL;
static void* heap_end = NULL;

// Helper function: Get chunk size
#define chunk_size(c) ((*((size_t*)c)) & ~(size_t)7)

// Helper function: Check if chunk is in use
int in_use(void* c) { 
    return *(unsigned int*)(c) & 0x1;
}

// Helper function: Mark a chunk
void mark(size_t* chunk) {
    (*chunk) |= 0x2;
}

// Helper function: Check if a chunk is marked
int is_marked(size_t* chunk) {
    return (*chunk) & 0x2;
}

// Helper function: Clear mark
void clear_mark(size_t* chunk) {
    (*chunk) &= ~(size_t)0x2;
}

// Helper function: Get next chunk
void* next_chunk(void* c) { 
    if (chunk_size(c) == 0) {
        fprintf(stderr, "Panic, chunk is of zero size.\n");
        return NULL;
    }
    if ((c + chunk_size(c)) < mem_heap_hi())
        return ((void*)c + chunk_size(c));
    else 
        return NULL;
}

// Simulated heap boundaries
void* mem_heap_lo(void) {
    return heap_start;
}

void* mem_heap_hi(void) {
    return heap_end;
}

// Simulated init_global_range (sets up .data section)
void init_global_range(void) {
    extern char __data_start, _end; // Linker-defined symbols
    data_mem.start = (size_t*)&__data_start;
    data_mem.end = (size_t*)&_end;
}

// Initialize garbage collector
void init_gc(void) {
    size_t stack_var;
    init_global_range();
    stack_mem.end = (size_t*)&stack_var;
    // Initialize heap by allocating a small block
    void* temp = malloc(16);
    if (temp) {
        heap_start = temp - sizeof(size_t); // Start at chunk header
        heap_end = heap_start + 1024 * 1024; // Simulate 1MB heap
        free(temp);
    }
}

// Run garbage collection
void gc(void) {
    size_t stack_var;
    stack_mem.start = (size_t*)&stack_var;
    // Mark phase: scan .data and stack
    walk_region_and_mark(data_mem.start, data_mem.end);
    walk_region_and_mark(stack_mem.start, stack_mem.end);
    // Sweep phase
    sweep();
}

// Check if a pointer points to a heap chunk
void* is_pointer(void* ptr) {
    if (ptr < mem_heap_lo() || ptr >= mem_heap_hi()) {
        return NULL;
    }
    void* chunk = mem_heap_lo();
    while (chunk && chunk < mem_heap_hi()) {
        size_t size = chunk_size(chunk);
        void* chunk_end = chunk + size;
        if (ptr >= (chunk + sizeof(size_t)) && ptr < chunk_end) {
            return chunk;
        }
        chunk = next_chunk(chunk);
    }
    return NULL;
}

// Walk a memory region and mark reachable chunks
void walk_region_and_mark(void* start, void* end) {
    for (size_t* current = (size_t*)start; current < (size_t*)end; current++) {
        void* potential_ptr = (void*)*current;
        void* chunk = is_pointer(potential_ptr);
        if (chunk && !is_marked(chunk)) {
            mark(chunk);
        }
    }
}

// Sweep heap, freeing unmarked in-use chunks
void sweep(void) {
    void* chunk = mem_heap_lo();
    while (chunk && chunk < mem_heap_hi()) {
        void* next = next_chunk(chunk);
        if (in_use(chunk) && !is_marked(chunk)) {
            free(chunk + sizeof(size_t));
        }
        if (in_use(chunk)) {
            clear_mark(chunk);
        }
        chunk = next;
    }
}
