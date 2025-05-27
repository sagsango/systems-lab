#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "vector.h"

/*
  This Layer will make sure that no assert fails in the vec_internal layer
  This Layer will not do any assertion
  This Layer will return error code instead.
  This Layer will make sure that user program which is doing vector operation,
do not crashes because of invalid operation.

  TODO:
      Replace all the assertion in this layer by returning errors.
*/

static void *vector_get_at(struct vector *vec, size_t index) {
  assert(index < vec->size);
  return vec->vec_in->get_at(vec->vec_in, index);
}

static int vector_put_at(struct vector *vec, size_t index, void *data) {
  assert(index < vec->size);
  return vec->vec_in->put_at(vec->vec_in, index, data);
}

/*
 * TODO:
 * This is not how resize algorithm works.
 * We should improve this logic
 */
static int vector_resize(struct vector *vec) {
  size_t size = vec->size;
  size_t capacity = vec->vec_in->get_capacity(vec->vec_in);
  if (capacity < size << 1) {
    vec->vec_in->resize(vec->vec_in, size<<1);
  }
  if (size >= VEC_MIN_CAPACITY && size  < capacity>>1) {
    vec->vec_in->resize(vec->vec_in, size);
  }
  return 0;
}

static int vector_push_back(struct vector *vec, void *data) {
  vector_resize(vec);
  size_t capacity = vec->vec_in->get_capacity(vec->vec_in);
  size_t index = vec->size;
  vec->vec_in->put_at(vec->vec_in, index, data);
  vec->size += 1;
  return 0;
}

static int vector_pop_back(struct vector *vec) {
  assert(vec->size);
  vec->size -= 1;
  vector_resize(vec);
  return 0;
}

static int vector_erase_at(struct vector *vec, size_t index) {
  /* TODO: shrink the capacity */
  assert(index < vec->size);
  if (index + 1 != vec->size) {
    memcpy(vec->vec_in->vec_data->arr + index,
           vec->vec_in->vec_data->arr + index + 1,
           (vec->size - (index + 1)) * sizeof(void *));
  }
  vec->size -= 1;
  vector_resize(vec);
  return 0;
}

static int vector_init(struct vector *vec, size_t size) {
  vec->vec_in->init(vec->vec_in,
                    size > VEC_MIN_CAPACITY ? size : VEC_MIN_CAPACITY);
  vec->size = size;
  return 0;
}

static  int vector_free(struct vector *vec) {
  vec->vec_in->free(vec->vec_in);
  vec->size = 0;
  return 0;
}
struct vector *vector_malloc() {
  struct vector *vec = (struct vector *)malloc(sizeof(struct vector));
  assert(NULL != vec);
  vec->vec_in  = vec_internal_malloc();
  vec->size = 0;
  vec->get_at = vector_get_at;
  vec->put_at = vector_put_at;
  vec->push_back = vector_push_back;
  vec->pop_back = vector_pop_back;
  vec->erase_at = vector_erase_at;
  vec->init = vector_init;
  vec->free = vector_free;
  return vec;
}