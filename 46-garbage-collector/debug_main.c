#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hw4.h"

int global_ptr1, global_ptr2;

void test_is_pointer(void) {
    init_gc();
    void* ptr = malloc(32);
    void* chunk = is_pointer(ptr);
    printf("Testing is_pointer: ptr=%p, chunk=%p\n", ptr, chunk);
    assert(chunk == ptr - sizeof(size_t));
    assert(is_pointer((void*)0x1) == NULL);
    free(ptr);
}

void test_walk_region_and_mark(void) {
    init_gc();
    void* ptr1 = malloc(32);
    void* ptr2 = malloc(64);
    global_ptr1 = (size_t)ptr1;
    size_t stack_ptr = (size_t)ptr2;
    walk_region_and_mark(&global_ptr1, &global_ptr1 + 1);
    walk_region_and_mark(&stack_ptr, &stack_ptr + 1);
    printf("Testing walk_region_and_mark:\n");
    printf("ptr1 marked: %d\n", is_marked(ptr1 - sizeof(size_t)));
    printf("ptr2 marked: %d\n", is_marked(ptr2 - sizeof(size_t)));
    assert(is_marked(ptr1 - sizeof(size_t)));
    assert(is_marked(ptr2 - sizeof(size_t)));
    free(ptr1);
    free(ptr2);
}

void test_sweep(void) {
    init_gc();
    void* ptr1 = malloc(32);
    void* ptr2 = malloc(64);
    void* ptr3 = malloc(128);
    global_ptr1 = (size_t)ptr1;
    size_t stack_ptr = (size_t)ptr2;
    walk_region_and_mark(&global_ptr1, &global_ptr1 + 1);
    walk_region_and_mark(&stack_ptr, &stack_ptr + 1);
    sweep();
    printf("Testing sweep:\n");
    printf("ptr1 %s valid\n", is_pointer(ptr1) ? "is" : "is not");
    printf("ptr2 %s valid\n", is_pointer(ptr2) ? "is" : "is not");
    printf("ptr3 %s valid\n", is_pointer(ptr3) ? "is" : "is not");
    assert(is_pointer(ptr1) != NULL);
    assert(is_pointer(ptr2) != NULL);
    assert(is_pointer(ptr3) == NULL);
    free(ptr1);
    free(ptr2);
}

int main(void) {
    printf("Running test_is_pointer...\n");
    test_is_pointer();
    printf("test_is_pointer passed!\n\n");
    printf("Running test_walk_region_and_mark...\n");
    test_walk_region_and_mark();
    printf("test_walk_region_and_mark passed!\n\n");
    printf("Running test_sweep...\n");
    test_sweep();
    printf("test_sweep passed!\n\n");
    printf("All tests passed!\n");
    return 0;
}
