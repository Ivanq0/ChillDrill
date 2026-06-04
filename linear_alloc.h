#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H

#include "allocator.h"
#include <stddef.h>

typedef struct {
  unsigned char *buffer;
  size_t size;
  size_t offset;
  size_t last_offset;
  size_t last_size;
} LinearCtx;

IAllocator create_linear_alloc(LinearCtx *context, void *memory,
                               size_t memory_size);

#endif