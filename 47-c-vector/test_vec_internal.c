#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vec_internal.h"

#define SIZE VEC_MIN_CAPACITY 
int main() {
    fprintf(stderr, "Starting test\n");
    struct vec_internal *vec = vec_internal_malloc();
    if (!vec) {
        fprintf(stderr, "Failed to allocate vec_internal\n");
        return 1;
    }
    fprintf(stderr, "Vector malloc success\n");

    if (vec->init(vec, SIZE) != 0) {
        fprintf(stderr, "Failed to initialize vec_internal\n");
        return 1;
    }
    fprintf(stderr, "Vector init success\n");

    printf("Initial capacity: %zu\n", vec->get_capacity(vec));

    for (size_t i = 0; i < SIZE; i++) {
        int *val = malloc(sizeof(int));
        *val = (int)(i + 1) * 10;
        vec->put_at(vec, i, val);
    }

    // Print items
    for (size_t i = 0; i < SIZE; i++) {
        int *val = vec->get_at(vec, i);
        if (val)
            printf("vec[%zu] = %d\n", i, *val);
        else
            printf("vec[%zu] = NULL\n", i);
    }

    // Resize
    printf("Resizing to capacity 10...\n");
    vec->resize(vec, 10);
    printf("New capacity: %zu\n", vec->get_capacity(vec));

    // Free data
    for (size_t i = 0; i < SIZE; i++) {
        free(vec->get_at(vec, i));  // Free each stored pointer
    }

    vec->free(vec);
    free(vec);

    return 0;
}

