#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include "allocator.h"
#include <stddef.h>

#define BUDDY_MAX_LEVELS 32

typedef struct BuddyNode {
  struct BuddyNode *next;
} BuddyNode;

typedef struct {
  unsigned char *buffer;
  size_t total_size;
  size_t min_block;
  int max_level;
  BuddyNode *free_lists[BUDDY_MAX_LEVELS];
  unsigned char *alloc_levels;
  size_t alloc_levels_count;
} BuddyCtx;

IAllocator create_buddy_alloc(BuddyCtx *buddy_context, void *memory_pool,
                              size_t pool_size, size_t min_block_size);

void destroy_buddy_alloc(BuddyCtx *buddy_context);

#endif