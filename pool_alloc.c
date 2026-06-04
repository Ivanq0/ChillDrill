#include "pool_alloc.h"

static void init_freelist(PoolCtx *ctx) {
  ctx->free_list = NULL;
  for (size_t i = 0; i < ctx->block_count; ++i) {
    FreeBlock *block = (FreeBlock *)(ctx->buffer + i * ctx->block_size);
    block->next = ctx->free_list;
    ctx->free_list = block;
  }
}

static void *pool_allocate(IAllocator *self, size_t bytes) {
  PoolCtx *ctx = (PoolCtx *)self->ctx;
  if (bytes == 0 || bytes > ctx->block_size || ctx->free_list == NULL)
    return NULL;
  FreeBlock *first = ctx->free_list;
  ctx->free_list = first->next;
  return first;
}

static void pool_deallocate(IAllocator *self, void *ptr) {
  if (ptr == NULL)
    return;
  PoolCtx *ctx = (PoolCtx *)self->ctx;
  unsigned char *p = (unsigned char *)ptr;
  if (p < ctx->buffer || p >= ctx->buffer + ctx->block_count * ctx->block_size)
    return;
  FreeBlock *block = (FreeBlock *)ptr;
  block->next = ctx->free_list;
  ctx->free_list = block;
}

static void pool_clear(IAllocator *self) {
  PoolCtx *ctx = (PoolCtx *)self->ctx;
  init_freelist(ctx);
}

IAllocator create_pool_alloc(PoolCtx *ctx, void *buffer, size_t buf_size,
                             size_t blk_size) {
  size_t aligned_block = align_up(blk_size);
  if (aligned_block < sizeof(FreeBlock))
    aligned_block = align_up(sizeof(FreeBlock));

  ctx->buffer = (unsigned char *)buffer;
  ctx->block_size = aligned_block;
  ctx->block_count = buf_size / aligned_block;
  ctx->free_list = NULL;
  init_freelist(ctx);

  IAllocator allocator;
  allocator.alloc = pool_allocate;
  allocator.free = pool_deallocate;
  allocator.realloc = stub_realloc;
  allocator.reset = pool_clear;
  allocator.ctx = ctx;
  return allocator;
}