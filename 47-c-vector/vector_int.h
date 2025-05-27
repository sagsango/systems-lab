#include <stdint.h>
#include <stddef.h>
#include "vector.h"

struct vector_int {
  struct vector *vec;
  int (*get_at)(struct vector_int *vec_int, size_t index);
  int (*put_at)(struct vector_int *vec_int, size_t index, int data);
  int (*push_back)(struct vector_int *vec_int, int data);
  int (*pop_back)(struct vector_int *vec_int);
  int (*erase_at)(struct vector_int *vec_int, size_t index);
  int (*init)(struct vector_int *vec_int, size_t size);
  int (*free)(struct vector_int *vec_int);
};

struct vector_int * vector_int_malloc();