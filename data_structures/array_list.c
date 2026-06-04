#include "array_list.h"
#include "helpers.h"
#include <string.h>

void array_list_init(ArrayList *arr, IAllocator *alc, size_t cap) {
  arr->alloc = alc;
  arr->size = 0;
  arr->capacity = cap > 0 ? cap : 4;
  arr->data = (void **)mem_alloc(alc, arr->capacity * sizeof(void *));
}

int array_list_add(ArrayList *arr, void *elem) {
  if (arr->size == arr->capacity) {
    size_t new_cap = arr->capacity * 2;
    size_t new_bytes = new_cap * sizeof(void *);
    size_t old_bytes = arr->capacity * sizeof(void *);

    void **new_ptr = (void **)mem_realloc(arr->alloc, arr->data, new_bytes);
    if (!new_ptr) {
      new_ptr = (void **)mem_alloc(arr->alloc, new_bytes);
      if (!new_ptr)
        return 0;
      if (arr->data) {
        memcpy(new_ptr, arr->data, old_bytes);
        mem_free(arr->alloc, arr->data);
      }
    }
    arr->data = new_ptr;
    arr->capacity = new_cap;
  }
  arr->data[arr->size++] = elem;
  return 1;
}

void *array_list_get(ArrayList *arr, size_t idx) {
  if (idx >= arr->size)
    return NULL;
  return arr->data[idx];
}

void array_list_destroy(ArrayList *arr) {
  if (arr->data) {
    mem_free(arr->alloc, arr->data);
    arr->data = NULL;
  }
  arr->size = 0;
  arr->capacity = 0;
}