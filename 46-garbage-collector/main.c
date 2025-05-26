#include <stdio.h>
#include <stdlib.h>
#include "hw4.h"

int global_ptr; // In .data section

int main(void) {
    init_gc();
    void* ptr1 = malloc(32);
    void* ptr2 = malloc(64);
    void* ptr3 = malloc(128);
    global_ptr = (size_t)ptr1;
    size_t stack_ptr = (size_t)ptr2;
    printf("Allocated ptr1: %p, ptr2: %p, ptr3: %p\n", ptr1, ptr2, ptr3);
    printf("Global ptr: %p, Stack ptr: %p\n", (void*)global_ptr, (void*)stack_ptr);
    gc();
    printf("After GC:\n");
    printf("ptr1 %s valid\n", is_pointer(ptr1) ? "is" : "is not");
    printf("ptr2 %s valid\n", is_pointer(ptr2) ? "is" : "is not");
    printf("ptr3 %s valid\n", is_pointer(ptr3) ? "is" : "is not");
    free(ptr1);
    free(ptr2);
    free(ptr3);
    return 0;
}
