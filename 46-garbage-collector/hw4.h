#include <stddef.h>

#ifndef HW4_H
#define HW4_H

// Structure to store memory region boundaries
typedef struct {
    size_t* start;
    size_t* end;
} mem_region_t;

// Global variables for memory regions
extern mem_region_t data_mem;
extern mem_region_t stack_mem;

// Function declarations
void* mem_heap_lo(void);
void* mem_heap_hi(void);
void init_global_range(void);
void init_gc(void);
void gc(void);
void* is_pointer(void* ptr);
void walk_region_and_mark(void* start, void* end);
void sweep(void);
int is_marked(size_t* chunk);
void mark(size_t* chunk);
void clear_mark(size_t* chunk);
int in_use(void* c);
void* next_chunk(void* c);

#endif
