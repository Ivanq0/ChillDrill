#include "allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int fail = 0;

static void check(char *name, int good) {
  printf("[%s] %s\n", good ? "Good" : "Bad", name);
  if (!good)
    fail++;
}
static void test_sys() {
  printf("== SysAlloc ==\n");
  IAllocator a = create_sys_alloc();
  int *p = (int *)i_alloc(&a, sizeof(int) * 4);
  check("alloc", p != NULL);
  if (!p)
    return;

  for (int i = 0; i < 4; ++i)
    p[i] = i + 3;
  int *q = (int *)i_realloc(&a, p, sizeof(int) * 8);
  check("realloc", q != NULL);
  check("save", q && q[2] == 5);
  i_free(&a, q);
}
int main(void) {
  test_sys();
  if (fail) {
    printf("Fail: %d\n", fail);
    return 1;
  }
  printf("Successful!\n");
  return 0;
}