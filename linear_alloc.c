#include "linear_alloc.h"
#include <string.h>

static void *linear_allocate(IAllocator *allocator, size_t bytes) {
  LinearCtx *ctx = (LinearCtx *)allocator->ctx;
  size_t aligned = align_up(bytes);
  if (ctx->offset + aligned > ctx->size)
    return NULL;
  void *result = ctx->buffer + ctx->offset;
  ctx->last_offset = ctx->offset;
  ctx->last_size = aligned;
  ctx->offset += aligned;
  return result;
}

static void *linear_reallocate(IAllocator *allocator, void *pointer,
                               size_t new_bytes) {
  LinearCtx *ctx = (LinearCtx *)allocator->ctx;
  if (pointer == NULL)
    return linear_allocate(allocator, new_bytes);
  size_t needed = align_up(new_bytes);
  unsigned char *ptr_as_byte = (unsigned char *)pointer;
  if (ptr_as_byte == ctx->buffer + ctx->last_offset) {
    if (ctx->last_offset + needed > ctx->size)
      return NULL;
    ctx->offset = ctx->last_offset + needed;
    ctx->last_size = needed;
    return pointer;
  }
  void *new_ptr = linear_allocate(allocator, new_bytes);
  if (new_ptr && new_bytes > 0)
    memcpy(new_ptr, pointer, new_bytes);
  return new_ptr;
}

static void linear_reset(IAllocator *allocator) {
  LinearCtx *ctx = (LinearCtx *)allocator->ctx;
  ctx->offset = 0;
  ctx->last_offset = 0;
  ctx->last_size = 0;
}

IAllocator create_linear_alloc(LinearCtx *context, void *memory_buffer,
                               size_t buffer_size) {
  context->buffer = (unsigned char *)memory_buffer;
  context->size = buffer_size;
  context->offset = 0;
  context->last_offset = 0;
  context->last_size = 0;
  IAllocator allocator;
  allocator.alloc = linear_allocate;
  allocator.free = stub_free;
  allocator.realloc = linear_reallocate;
  allocator.reset = linear_reset;
  allocator.ctx = context;
  return allocator;
}