#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"


int main() {
    fprintf(stderr,"Test Starts\n");
    struct vector *v = vector_malloc();
    if (!v) {
        fprintf(stderr, "Failed to allocate vector.\n");
        return 1;
    }
    fprintf(stderr,"vector mallac sucess\n");

    if (v->init(v, 0) != 0) {
        fprintf(stderr, "Vector init failed.\n");
        return 1;
    }
    fprintf(stderr,"vector init sucess\n");

     
    // Push integers 10, 20, 30
    for (int i = 0; i < 3; i++) {
        int *val = malloc(sizeof(int));
        *val = (i + 1) * 10;
        v->push_back(v, val);
    }

    printf("Vector contents:\n");
    for (size_t i = 0; i < v->size; i++) {
        int *val = (int *)v->get_at(v, i);
        if (val) printf("  [%zu] = %d\n", i, *val);
    }

    // Erase index 1 (value 20)
    free(v->get_at(v, 1));
    printf("\nErasing index 1...\n");
    v->erase_at(v, 1);

    printf("After erase:\n");
    for (size_t i = 0; i < v->size; i++) {
        int *val = (int *)v->get_at(v, i);
        if (val) printf("  [%zu] = %d\n", i, *val);
    }

    // Pop the last element
    printf("\nPopping last element...\n");
    v->pop_back(v);

    printf("After pop:\n");
    for (size_t i = 0; i < v->size; i++) {
        int *val = (int *)v->get_at(v, i);
        if (val) printf("  [%zu] = %d\n", i, *val);
    }

    // Free memory used by vector
    v->free(v);
    //free(v); // No need

    return 0;
}
