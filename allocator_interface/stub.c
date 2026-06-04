#include "stub.h"
#include "allocator.h"

void noop_free(IAllocator *alloc, void *mem) {
  (void)alloc;
  (void)mem;
}

void *noop_realloc(IAllocator *alloc, void *mem, size_t bytes) {
  (void)alloc;
  (void)mem;
  (void)bytes;
  return NULL;
}

void noop_reset(IAllocator *alloc) { (void)alloc; }