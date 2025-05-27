#include <stddef.h>

#define VEC_MIN_CAPACITY 2

struct vec_data {
  void **arr;
  size_t arr_capacity;
};

struct vec_internal {
  struct vec_data *vec_data;
  int (*init)(struct vec_internal *vec, size_t capacity);
  size_t (*get_capacity)(struct vec_internal *vec);
  void *(*get_at)(struct vec_internal *vec, size_t index);
  int (*put_at)(struct vec_internal *vec, size_t index, void *data);
  int (*resize)(struct vec_internal *vec, int capacity);
  int (*free)(struct vec_internal*vec);
};

struct vec_internal * vec_internal_malloc();