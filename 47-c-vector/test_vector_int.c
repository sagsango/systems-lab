#include <stdio.h>
#include <stdlib.h>
#include "vector_int.h"

// Assuming vector_int is declared in vector.h or another included file
struct vector_int *vector_int_malloc();

int main() {
    struct vector_int *vec_int = vector_int_malloc();
    fprintf(stderr, "Starting test\n");
    if (!vec_int) {
        fprintf(stderr, "Failed to allocate vector_int\n");
        return 1;
    }
    fprintf(stderr, "vector_int malloc success\n");

    if (vec_int->init(vec_int, 0) != 0) {
        fprintf(stderr, "Failed to initialize vector_int\n");
        return 1;
    }
    fprintf(stderr, "vector_int int success\n");

    // Add integers
    for (int i = 0; i < 5; ++i) {
        if (vec_int->push_back(vec_int, i * 10) != 0) {
            fprintf(stderr, "Failed to push_back %d\n", i * 10);
        }
    }
    

    // Get and print elements
    for (size_t i = 0; i < 5; ++i) {
        int value = vec_int->get_at(vec_int, i);
        printf("vec_int[%zu] = %d\n", i, value);
    }

    // Erase element at index 2
    printf("Erasing element at index 2...\n");
    vec_int->erase_at(vec_int, 2);

    // Print after erase
    for (size_t i = 0; i < 4; ++i) {
        int value = vec_int->get_at(vec_int, i);
        printf("vec_int[%zu] = %d\n", i, value);
    }

    // Pop last
    printf("Popping last element...\n");
    vec_int->pop_back(vec_int);

    // Final contents
    for (size_t i = 0; i < 3; ++i) {
        int value = vec_int->get_at(vec_int, i);
        printf("vec_int[%zu] = %d\n", i, value);
    }

    vec_int->free(vec_int);
    free(vec_int);

    return 0;
}
