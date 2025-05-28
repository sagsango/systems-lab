#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "vec_internal.h"

static int vec_internal_init(struct vec_internal *vec, size_t capacity) {
  assert(NULL == vec->vec_data->arr);
  vec->vec_data->arr = (void*)malloc(sizeof(void **) * capacity);
  assert(NULL != vec->vec_data->arr);
  vec->vec_data->arr_capacity = capacity;
  return 0;
}

static size_t vec_internal_get_capacity(struct vec_internal *vec) {
  return vec->vec_data->arr_capacity;
}

void *vec_internal_get_at(struct vec_internal *vec, size_t index) {
  assert(index < vec->vec_data->arr_capacity);
  return vec->vec_data->arr[index];
}

int vec_internal_put_at(struct vec_internal *vec, size_t index, void *data) {
 assert(index < vec->vec_data->arr_capacity);
 vec->vec_data->arr[index] = data;
 return 0;
}

static int vec_internal_resize(struct vec_internal *vec, int capacity) {
  assert(vec->vec_data->arr_capacity);
  /* capacity and be grater or smaller than before as required */
  vec->vec_data->arr = realloc(vec->vec_data->arr, (capacity)* sizeof(void**));
  vec->vec_data->arr_capacity = capacity;
  assert(NULL != vec->vec_data->arr);
  return 0;
}

static int vec_internal_free(struct vec_internal *vec) {
  free(vec->vec_data->arr);
  vec->vec_data->arr = NULL;
  vec->vec_data->arr_capacity = 0;
  free(vec->vec_data);
  free(vec);
  return 0;
}

struct vec_internal *vec_internal_malloc() {
  struct vec_internal *vec;
  vec = (struct vec_internal *)malloc(sizeof(struct vec_internal));
  assert(NULL != vec);
  vec->vec_data = (struct vec_data *)malloc(sizeof(struct vec_data));
  assert (NULL != vec->vec_data);
  vec->vec_data->arr = NULL;
  vec->vec_data->arr_capacity = 0;
  vec->init = vec_internal_init;
  vec->get_capacity = vec_internal_get_capacity;
  vec->get_at = vec_internal_get_at;
  vec->put_at = vec_internal_put_at;
  vec->resize = vec_internal_resize;
  vec->free = vec_internal_free;
  return vec;
}