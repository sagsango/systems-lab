#include <stdint.h>
#include <stddef.h>
#include "vec_internal.h"

struct vector {
  struct vec_internal *vec_in;
  size_t size;
  void *(*get_at)(struct vector *vec, size_t index);
  int (*put_at)(struct vector *vec, size_t index, void *data);
  int (*push_back)(struct vector *vec, void *data);
  int (*pop_back)(struct vector*vec);
  int (*erase_at)(struct vector *vec, size_t index);
  int (*init)(struct vector *vec, size_t size);
  int (*free)(struct vector *vec);
};

struct vector * vector_malloc();