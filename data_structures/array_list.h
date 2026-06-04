#ifndef ARRAY_LIST_MODULE_H
#define ARRAY_LIST_MODULE_H

#include "allocator.h"
#include <stddef.h>

typedef struct {
  IAllocator *alloc;
  void **data;
  size_t size;
  size_t capacity;
} ArrayList;

void array_list_init(ArrayList *arr, IAllocator *alc, size_t cap);
int array_list_add(ArrayList *arr, void *elem);
void *array_list_get(ArrayList *arr, size_t idx);
void array_list_destroy(ArrayList *arr);

#endif