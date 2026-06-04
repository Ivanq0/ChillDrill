#include "stack.h"
#include <assert.h>
#include <stdio.h>

void test_stack_init_and_empty(void) {
  Stack s;
  initStack(&s);
  assert(isEmpty(&s) == 1);
}

void test_stack_push_pop(void) {
  Stack s;
  int value;

  initStack(&s);

  assert(push(&s, 42) == 1);
  assert(push(&s, 15) == 1);
  assert(isEmpty(&s) == 0);

  assert(pop(&s, &value) == 1);
  assert(value == 15);

  assert(pop(&s, &value) == 1);
  assert(value == 42);

  assert(isEmpty(&s) == 1);
}

void test_stack_overflow(void) {
  Stack s;
  initStack(&s);

  for (int i = 0; i < MAX_SIZE; i++) {
    assert(push(&s, i) == 1);
  }

  assert(isFull(&s) == 1);
  assert(isEmpty(&s) == 0);
  assert(push(&s, 999) == 0);
}

void test_stack_underflow(void) {
  Stack s;
  int value;

  initStack(&s);

  assert(pop(&s, &value) == 0);
}

int main(void) {
  printf("Running stack tests...\n");

  test_stack_init_and_empty();
  printf("✓ test_stack_init_and_empty passed\n");

  test_stack_push_pop();
  printf("✓ test_stack_push_pop passed\n");

  test_stack_overflow();
  printf("✓ test_stack_overflow passed\n");

  test_stack_underflow();
  printf("✓ test_stack_underflow passed\n");

  printf("\nAll tests passed successfully!\n");
  return 0;
}
