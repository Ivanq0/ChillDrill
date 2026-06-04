#include "buddy_alloc.h"
#include <stdlib.h>
#include <string.h>

static int is_power_of_two(size_t value) {
  return value && ((value & (value - 1)) == 0);
}

static size_t floor_power_of_two(size_t x) {
  size_t result = 1;
  while ((result << 1) && (result << 1) <= x)
    result <<= 1;
  return result;
}

static int log2_of_size(size_t n) {
  int level = 0;
  while (n > 1) {
    n >>= 1;
    level++;
  }
  return level;
}

static int compute_level(size_t need, size_t min_block) {
  if (need < min_block)
    need = min_block;
  size_t block_size = min_block;
  int lvl = 0;
  while (block_size < need) {
    block_size <<= 1;
    lvl++;
  }
  return lvl;
}

static int remove_from_freelist(BuddyCtx *ctx, int level, BuddyNode *node) {
  BuddyNode **current = &ctx->free_lists[level];
  while (*current) {
    if (*current == node) {
      *current = node->next;
      return 1;
    }
    current = &(*current)->next;
  }
  return 0;
}

static void *buddy_allocate(IAllocator *self, size_t requested_size) {
  BuddyCtx *ctx = (BuddyCtx *)self->ctx;
  if (requested_size == 0)
    return NULL;

  int target_level = compute_level(requested_size, ctx->min_block);
  if (target_level > ctx->max_level)
    return NULL;

  int level = target_level;
  while (level <= ctx->max_level && ctx->free_lists[level] == NULL)
    level++;
  if (level > ctx->max_level)
    return NULL;

  BuddyNode *block = ctx->free_lists[level];
  ctx->free_lists[level] = block->next;
  while (level > target_level) {
    level--;
    size_t half_size = ctx->min_block << level;
    BuddyNode *buddy = (BuddyNode *)((unsigned char *)block + half_size);
    buddy->next = ctx->free_lists[level];
    ctx->free_lists[level] = buddy;
  }

  size_t offset = (unsigned char *)block - ctx->buffer;
  ctx->alloc_levels[offset / ctx->min_block] =
      (unsigned char)(target_level + 1);
  return block;
}

static void buddy_deallocate(IAllocator *self, void *pointer) {
  if (pointer == NULL)
    return;
  BuddyCtx *ctx = (BuddyCtx *)self->ctx;
  unsigned char *p = (unsigned char *)pointer;
  if (p < ctx->buffer || p >= ctx->buffer + ctx->total_size)
    return;

  size_t offset = p - ctx->buffer;
  size_t index = offset / ctx->min_block;
  int level = (int)ctx->alloc_levels[index] - 1;
  if (level < 0)
    return;
  ctx->alloc_levels[index] = 0;

  while (level < ctx->max_level) {
    size_t block_size = ctx->min_block << level;
    size_t buddy_offset = offset ^ block_size; // XOR для нахождения партнёра
    BuddyNode *buddy = (BuddyNode *)(ctx->buffer + buddy_offset);
    if (!remove_from_freelist(ctx, level, buddy))
      break;
    if (buddy_offset < offset)
      offset = buddy_offset;
    level++;
  }

  BuddyNode *freed_block = (BuddyNode *)(ctx->buffer + offset);
  freed_block->next = ctx->free_lists[level];
  ctx->free_lists[level] = freed_block;
}

static void *buddy_resize(IAllocator *self, void *pointer, size_t new_size) {
  BuddyCtx *ctx = (BuddyCtx *)self->ctx;
  if (pointer == NULL)
    return buddy_allocate(self, new_size);
  if (new_size == 0) {
    buddy_deallocate(self, pointer);
    return NULL;
  }

  size_t offset = (unsigned char *)pointer - ctx->buffer;
  size_t index = offset / ctx->min_block;
  int level = (int)ctx->alloc_levels[index] - 1;
  if (level < 0)
    return NULL;

  size_t current_size = ctx->min_block << level;
  if (new_size <= current_size)
    return pointer;

  void *new_block = buddy_allocate(self, new_size);
  if (new_block) {
    memcpy(new_block, pointer, current_size);
    buddy_deallocate(self, pointer);
  }
  return new_block;
}

static void buddy_reset(IAllocator *self) {
  BuddyCtx *ctx = (BuddyCtx *)self->ctx;
  for (int i = 0; i < BUDDY_MAX_LEVELS; ++i)
    ctx->free_lists[i] = NULL;
  memset(ctx->alloc_levels, 0, ctx->alloc_levels_count);
  BuddyNode *root = (BuddyNode *)ctx->buffer;
  root->next = NULL;
  ctx->free_lists[ctx->max_level] = root;
}

IAllocator create_buddy_alloc(BuddyCtx *ctx, void *buffer, size_t total_size,
                              size_t min_block) {
  if (!is_power_of_two(total_size))
    total_size = floor_power_of_two(total_size);
  if (min_block < sizeof(BuddyNode))
    min_block = sizeof(BuddyNode);
  if (!is_power_of_two(min_block))
    min_block = floor_power_of_two(min_block);

  ctx->buffer = (unsigned char *)buffer;
  ctx->total_size = total_size;
  ctx->min_block = min_block;
  ctx->max_level = log2_of_size(total_size / min_block);

  for (int i = 0; i < BUDDY_MAX_LEVELS; ++i)
    ctx->free_lists[i] = NULL;

  ctx->alloc_levels_count = total_size / min_block;
  ctx->alloc_levels = (unsigned char *)calloc(ctx->alloc_levels_count, 1);

  BuddyNode *root_block = (BuddyNode *)buffer;
  root_block->next = NULL;
  ctx->free_lists[ctx->max_level] = root_block;

  IAllocator allocator;
  allocator.alloc = buddy_allocate;
  allocator.free = buddy_deallocate;
  allocator.realloc = buddy_resize;
  allocator.reset = buddy_reset;
  allocator.ctx = ctx;
  return allocator;
}

void destroy_buddy_alloc(BuddyCtx *ctx) {
  free(ctx->alloc_levels);
  ctx->alloc_levels = NULL;
  ctx->alloc_levels_count = 0;
}