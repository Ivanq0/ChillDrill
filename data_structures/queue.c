#include "queue.h"
#include "helpers.h"

void queue_init(Queue *queue, IAllocator *alloc) {
  queue->allocator = alloc;
  queue->head = NULL;
  queue->tail = NULL;
}

int queue_push(Queue *queue, void *value) {
  Node *node = (Node *)mem_alloc(queue->allocator, sizeof(Node));
  if (!node)
    return 0;
  node->data = value;
  node->next = NULL;
  if (queue->tail == NULL) {
    queue->head = node;
    queue->tail = node;
  } else {
    queue->tail->next = node;
    queue->tail = node;
  }
  return 1;
}

void *queue_pop(Queue *queue) {
  if (queue->head == NULL)
    return NULL;
  Node *tmp = queue->head;
  void *result = tmp->data;
  queue->head = queue->head->next;
  if (queue->head == NULL)
    queue->tail = NULL;
  mem_free(queue->allocator, tmp);
  return result;
}

int queue_is_empty(Queue *queue) { return queue->head == NULL; }

void queue_destroy(Queue *queue) {
  while (!queue_is_empty(queue))
    queue_pop(queue);
}