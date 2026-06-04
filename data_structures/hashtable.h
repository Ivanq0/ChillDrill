#ifndef HASH_TABLE_MODULE_H
#define HASH_TABLE_MODULE_H

#include "allocator.h"
#include <stddef.h>

typedef struct HashNode {
  const char *key;
  void *value;
  struct HashNode *next;
} HashNode;

typedef struct {
  IAllocator *alloc;
  HashNode **buckets;
  size_t num_buckets;
} HashTable;

void hash_table_init(HashTable *ht, IAllocator *alc, size_t bucket_count);
int hash_table_insert(HashTable *ht, const char *k, void *v);
void *hash_table_get(HashTable *ht, const char *k);
int hash_table_remove(HashTable *ht, const char *k);
void hash_table_destroy(HashTable *ht);

#endif