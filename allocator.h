#include <stddef.h>

typedef struct IAllocator {
  void *(*alloc)(struct IAllocator *self, size_t size);
  void (*free)(struct IAllocator *self, void *ptr);
  void *(*realloc)(struct IAllocator *self, void *ptr, size_t new_size);
  void (*reset)(struct IAllocator *self);
  void *ctx;
} IAllocator;

void stub_free(IAllocator *self, void *ptr);
void *stub_realloc(IAllocator *self, void *ptr, size_t new_size);
void stub_reset(IAllocator *self);

IAllocator create_sys_alloc(void);

static inline void *i_alloc(IAllocator *a, size_t size) {
  return a->alloc(a, size);
}
static inline void i_free(IAllocator *a, void *ptr) { a->free(a, ptr); }
static inline void *i_realloc(IAllocator *a, void *ptr, size_t size) {
  return a->realloc(a, ptr, size);
}
static inline void i_reset(IAllocator *a) { a->reset(a); }