#ifndef QUEUE_MODULE_H
#define QUEUE_MODULE_H

#include "allocator.h"
#include <stddef.h>

typedef struct Node {
  void *data;
  struct Node *next;
} Node;

typedef struct {
  IAllocator *allocator;
  Node *head;
  Node *tail;
} Queue;

void queue_init(Queue *queue, IAllocator *alloc);
int queue_push(Queue *queue, void *value);
void *queue_pop(Queue *queue);
int queue_is_empty(Queue *queue);
void queue_destroy(Queue *queue);

#endif