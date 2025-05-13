

/*
 * 
 * Fences provides flaxibility to decide the memory order after bunch
 * of Atomic opertions.
 * 
 * 
 * 
 * https://en.cppreference.com/w/cpp/atomic/atomic_thread_fence
 * 
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>

// Mock function that simulates computation and returns a string
char* computation(int i) {
    char *buf = malloc(32);
    snprintf(buf, 32, "data[%d]", i);
    return buf;
}

// Print function
void print(const char *s) {
    printf("%s\n", s);
}

// Global variables
_Atomic int arr[3] = { -1, -1, -1 };
char* data[1000];  // non-atomic data

// Thread A: compute 3 values
void ThreadA(int v0, int v1, int v2) {
    // Assume 0 <= v0,v1,v2 < 1000
    data[v0] = computation(v0);
    data[v1] = computation(v1);
    data[v2] = computation(v2);

    atomic_thread_fence(memory_order_release);

    atomic_store_explicit(&arr[0], v0, memory_order_relaxed);
    atomic_store_explicit(&arr[1], v1, memory_order_relaxed);
    atomic_store_explicit(&arr[2], v2, memory_order_relaxed);
}

// Thread B: reads and prints computed values
void ThreadB() {
    int v0 = atomic_load_explicit(&arr[0], memory_order_relaxed);
    int v1 = atomic_load_explicit(&arr[1], memory_order_relaxed);
    int v2 = atomic_load_explicit(&arr[2], memory_order_relaxed);

    atomic_thread_fence(memory_order_acquire);

    if (v0 != -1)
        print(data[v0]);
    if (v1 != -1)
        print(data[v1]);
    if (v2 != -1)
        print(data[v2]);
}
