#include "sys_alloc.h"
#include "allocator.h"
#include <stdlib.h>

static void *allocate_memory(IAllocator *alloc, size_t bytes) {
  (void)alloc;
  return malloc(bytes);
}

static void release_memory(IAllocator *alloc, void *ptr) {
  (void)alloc;
  free(ptr);
}

static void *resize_memory(IAllocator *alloc, void *ptr, size_t new_bytes) {
  (void)alloc;
  return realloc(ptr, new_bytes);
}

static void reset_allocator(IAllocator *alloc) { (void)alloc; }

IAllocator create_sys_alloc(void) {
  IAllocator a;
  a.alloc = allocate_memory;
  a.free = release_memory;
  a.realloc = resize_memory;
  a.reset = reset_allocator;
  a.ctx = NULL;
  return a;
}