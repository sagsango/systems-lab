#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "vector_int.h"


static int vector_int_get_at(struct vector_int *vec_int, size_t index) {
  int *ptr = vec_int->vec->get_at(vec_int->vec, index);
  assert (NULL != ptr);
  return *ptr;
}

static int vector_int_put_at(struct vector_int *vec_int, size_t index, int data) {
  void *ptr = malloc(sizeof(int));
  assert(NULL != ptr);
  vec_int->vec->put_at(vec_int->vec, index, ptr);
  *((int*)ptr) = data;
  return 0;
}

static int vector_int_push_back(struct vector_int *vec_int, int data) {
 void *ptr = malloc(sizeof(int));
 assert(NULL != ptr);
 vec_int->vec->push_back(vec_int->vec, ptr);
 *((int*)ptr) = data; 
 return 0;
}

static int vector_int_pop_back(struct vector_int *vec_int) {
  assert(0 != vec_int->vec->size);
  size_t last_index = vec_int->vec->size - 1;
  void *data = vec_int->vec->get_at(vec_int->vec, last_index);
  assert (data);
  free(data);
  vec_int->vec->pop_back(vec_int->vec);
  return 0;
}

static int vector_int_erase_at(struct vector_int *vec_int, size_t index) {
  void *data = vec_int->vec->get_at(vec_int->vec, index);
  assert(NULL != data);
  free(data);
  vec_int->vec->erase_at(vec_int->vec, index);
  return 0;
}

static int vector_int_init(struct vector_int *vec_int, size_t size) {
  vec_int->vec->init(vec_int->vec, size);
  return 0;
}

static  int vector_int_free(struct vector_int *vec_int) {
  size_t size = vec_int->vec->size;
  for (size_t i = 0; i < size; ++i) {
    free(vec_int->vec->get_at(vec_int->vec, i));
  }
  vec_int->vec->free(vec_int->vec);
  return 0;
}

struct vector_int *vector_int_malloc() {
  struct vector_int *vec_int = (struct vector_int *)malloc(sizeof(struct vector_int));
  assert(NULL != vec_int);
  vec_int->vec = vector_malloc();
  vec_int->get_at = vector_int_get_at;
  vec_int->put_at = vector_int_put_at;
  vec_int->push_back = vector_int_push_back;
  vec_int->pop_back = vector_int_pop_back;
  vec_int->erase_at = vector_int_erase_at;
  vec_int->init = vector_int_init;
  vec_int->free = vector_int_free;
  return vec_int;
}