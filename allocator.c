#include "allocator.h"
#include <stdlib.h>

// Заглушки для нереализованных операций (необходимы для других аллокаторов)
void stub_free(IAllocator *alloc, void *mem) {
  (void)alloc;
  (void)mem;
}

void *stub_realloc(IAllocator *alloc, void *mem, size_t new_size) {
  (void)alloc;
  (void)mem;
  (void)new_size;
  return NULL;
}

void stub_reset(IAllocator *alloc) { (void)alloc; }

// Вспомогательные функции для системного аллокатора (malloc/realloc/free)
static void *allocate_sys(IAllocator *alloc, size_t bytes) {
  (void)alloc;
  return malloc(bytes);
}

static void free_sys(IAllocator *alloc, void *ptr) {
  (void)alloc;
  free(ptr);
}

static void *realloc_sys(IAllocator *alloc, void *ptr, size_t new_bytes) {
  (void)alloc;
  return realloc(ptr, new_bytes);
}

static void reset_sys(IAllocator *alloc) { (void)alloc; }

// Создание экземпляра системного аллокатора
IAllocator create_sys_alloc(void) {
  IAllocator allocator;
  allocator.alloc = allocate_sys;
  allocator.free = free_sys;
  allocator.realloc = realloc_sys;
  allocator.reset = reset_sys;
  allocator.ctx = NULL;
  return allocator;
}