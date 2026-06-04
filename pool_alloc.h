#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include "allocator.h"
#include <stddef.h>

typedef struct FreeBlock {
  struct FreeBlock *next;
} FreeBlock;

typedef struct {
  unsigned char *buffer;
  size_t block_size;
  size_t block_count;
  FreeBlock *free_list;
} PoolCtx;

IAllocator create_pool_alloc(PoolCtx *context, void *memory_pool,
                             size_t pool_bytes, size_t chunk_size);

#endif