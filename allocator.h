#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

typedef struct IAllocator {
  void *(*alloc)(struct IAllocator *allocator, size_t size);
  void (*free)(struct IAllocator *allocator, void *pointer);
  void *(*realloc)(struct IAllocator *allocator, void *pointer,
                   size_t new_size);
  void (*reset)(struct IAllocator *allocator);
  void *ctx;
} IAllocator;

static inline size_t align_up(size_t s) { return (s + 7) & ~7; }

void stub_free(IAllocator *allocator, void *pointer);
void *stub_realloc(IAllocator *allocator, void *pointer, size_t new_size);
void stub_reset(IAllocator *allocator);

IAllocator create_sys_alloc(void);

static inline void *i_alloc(IAllocator *alloc, size_t bytes) {
  return alloc->alloc(alloc, bytes);
}

static inline void i_free(IAllocator *alloc, void *block) {
  alloc->free(alloc, block);
}

static inline void *i_realloc(IAllocator *alloc, void *block,
                              size_t new_bytes) {
  return alloc->realloc(alloc, block, new_bytes);
}

static inline void i_reset(IAllocator *alloc) { alloc->reset(alloc); }

#endif