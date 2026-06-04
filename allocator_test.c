#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allocator.h"
#include "buddy_alloc.h"
#include "linear_alloc.h"
#include "pool_alloc.h"

static int errors = 0;

static void report(const char *test_name, int condition) {
  if (condition) {
    printf("[Good] %s\n", test_name);
  } else {
    printf("[Bad]  %s\n", test_name);
    errors++;
  }
}

static void check_system_allocator() {
  printf("== SysAlloc ==\n");
  IAllocator alloc = create_sys_alloc();

  int *data = (int *)i_alloc(&alloc, sizeof(int) * 4);
  report("alloc", data != NULL);
  if (!data)
    return;

  for (int idx = 0; idx < 4; ++idx)
    data[idx] = idx + 3;

  int *new_data = (int *)i_realloc(&alloc, data, sizeof(int) * 8);
  report("realloc", new_data != NULL);
  if (new_data)
    report("save", new_data[2] == 5);

  i_free(&alloc, new_data);
}

static void verify_linear_alignment(void *p2, void *p3) {
  int align_ok = (((size_t)p2 & 7) == 0) && (((size_t)p3 & 7) == 0);
  report("alignment 8", align_ok);
}

static void linear_tests() {
  printf("== Linear ==\n");
  unsigned char memory[256];
  LinearCtx context;
  IAllocator allocator = create_linear_alloc(&context, memory, sizeof(memory));

  void *first = i_alloc(&allocator, 10);
  void *second = i_alloc(&allocator, 30);
  void *third = i_alloc(&allocator, 7);
  report("alloc != NULL", first && second && third);
  report("p2 > p1", (unsigned char *)second > (unsigned char *)first);
  verify_linear_alignment(second, third);

  void *oversized = i_alloc(&allocator, 1024);
  report("overflow -> NULL", oversized == NULL);

  void *g = i_alloc(&allocator, 16);
  void *g2 = i_realloc(&allocator, g, 64);
  report("realloc last in place", g == g2);

  i_reset(&allocator);
  void *after_reset = i_alloc(&allocator, 8);
  report("after reset = start", after_reset == memory);
}

static void pool_tests() {
  printf("== Pool ==\n");
  unsigned char buffer[256];
  PoolCtx pool_context;
  IAllocator pool_alloc =
      create_pool_alloc(&pool_context, buffer, sizeof(buffer), 32);

  void *handles[8];
  int all_ok = 1;
  int idx;
  for (idx = 0; idx < 8; ++idx) {
    handles[idx] = i_alloc(&pool_alloc, 32);
    if (!handles[idx])
      all_ok = 0;
  }
  report("8 blocks of 32", all_ok);
  report("overflow -> NULL", i_alloc(&pool_alloc, 32) == NULL);

  i_free(&pool_alloc, handles[3]);
  i_free(&pool_alloc, handles[5]);
  void *new_a = i_alloc(&pool_alloc, 32);
  void *new_b = i_alloc(&pool_alloc, 32);
  report("reuse after free", new_a && new_b);

  report("too big -> NULL", i_alloc(&pool_alloc, 64) == NULL);

  i_reset(&pool_alloc);
  int can_reuse_all = 1;
  for (idx = 0; idx < 8; ++idx) {
    if (!i_alloc(&pool_alloc, 32))
      can_reuse_all = 0;
  }
  report("reset restores all blocks", can_reuse_all);
}

static void buddy_alloc_tests() {
  printf("== Buddy ==\n");
  size_t region_size = 1024;
  unsigned char *raw_mem = (unsigned char *)malloc(region_size);
  report("test buffer allocated", raw_mem != NULL);
  if (!raw_mem)
    return;

  BuddyCtx buddy_ctx;
  IAllocator buddy_alloc =
      create_buddy_alloc(&buddy_ctx, raw_mem, region_size, 32);

  void *item1 = i_alloc(&buddy_alloc, 60);
  void *item2 = i_alloc(&buddy_alloc, 100);
  void *item3 = i_alloc(&buddy_alloc, 30);
  report("3 allocs", item1 && item2 && item3);
  report("distinct", (item1 != item2) && (item2 != item3) && (item1 != item3));

  i_free(&buddy_alloc, item1);
  i_free(&buddy_alloc, item3);
  i_free(&buddy_alloc, item2);

  void *full_block = i_alloc(&buddy_alloc, 1024);
  report("merged back to full block", full_block == raw_mem);
  i_free(&buddy_alloc, full_block);

  void *small = i_alloc(&buddy_alloc, 30);
  report("alloc before realloc", small != NULL);
  if (small) {
    memcpy(small, "hello", 6);
    void *grown = i_realloc(&buddy_alloc, small, 200);
    report("realloc grow", grown != NULL);
    if (grown) {
      int data_ok = (memcmp(grown, "hello", 6) == 0);
      report("realloc keeps data", data_ok);
      i_free(&buddy_alloc, grown);
    }
  }

  void *reserved[64];
  int fill_count = 0;
  for (int i = 0; i < 64; ++i) {
    reserved[i] = i_alloc(&buddy_alloc, 32);
    if (reserved[i])
      fill_count++;
  }
  report("fill all min blocks", fill_count == 32);
  for (int i = 0; i < 64; ++i)
    if (reserved[i])
      i_free(&buddy_alloc, reserved[i]);

  destroy_buddy_alloc(&buddy_ctx);
  free(raw_mem);
}

int main() {
  check_system_allocator();
  buddy_alloc_tests();
  linear_tests();
  pool_tests();

  if (errors) {
    printf("Fail: %d\n", errors);
    return 1;
  }
  printf("Successful!\n");
  return 0;
}