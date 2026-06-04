#ifndef STUB_HELPERS_H
#define STUB_HELPERS_H

#include "allocator.h"
#include <stddef.h>

void noop_free(IAllocator *alloc, void *ptr);
void *noop_realloc(IAllocator *alloc, void *ptr, size_t size);
void noop_reset(IAllocator *alloc);

#endif