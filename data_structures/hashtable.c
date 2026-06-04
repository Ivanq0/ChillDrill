#include "hashtable.h"
#include "helpers.h"
#include <string.h>

static unsigned int hash_key(const char *s, size_t limit) {
  unsigned int h = 0;
  while (*s) {
    h = h * 65599 + *s++;
  }
  return h % limit;
}

void hash_table_init(HashTable *ht, IAllocator *alc, size_t buckets) {
  ht->alloc = alc;
  ht->num_buckets = buckets > 0 ? buckets : 16;
  ht->buckets =
      (HashNode **)mem_alloc(alc, ht->num_buckets * sizeof(HashNode *));
  if (ht->buckets) {
    for (size_t i = 0; i < ht->num_buckets; ++i) {
      ht->buckets[i] = NULL;
    }
  }
}

int hash_table_insert(HashTable *ht, const char *k, void *val) {
  if (!ht->buckets)
    return 0;

  unsigned int idx = hash_key(k, ht->num_buckets);
  HashNode *node = ht->buckets[idx];
  while (node) {
    if (strcmp(node->key, k) == 0) {
      node->value = val;
      return 1;
    }
    node = node->next;
  }

  HashNode *new_node = (HashNode *)mem_alloc(ht->alloc, sizeof(HashNode));
  if (!new_node)
    return 0;

  new_node->key = k;
  new_node->value = val;
  new_node->next = ht->buckets[idx];
  ht->buckets[idx] = new_node;
  return 1;
}

void *hash_table_get(HashTable *ht, const char *k) {
  if (!ht->buckets)
    return NULL;

  unsigned int idx = hash_key(k, ht->num_buckets);
  HashNode *node = ht->buckets[idx];
  while (node) {
    if (strcmp(node->key, k) == 0)
      return node->value;
    node = node->next;
  }
  return NULL;
}

int hash_table_remove(HashTable *ht, const char *k) {
  if (!ht->buckets)
    return 0;

  unsigned int idx = hash_key(k, ht->num_buckets);
  HashNode *node = ht->buckets[idx];
  HashNode *prev = NULL;

  while (node) {
    if (strcmp(node->key, k) == 0) {
      if (prev)
        prev->next = node->next;
      else
        ht->buckets[idx] = node->next;
      mem_free(ht->alloc, node);
      return 1;
    }
    prev = node;
    node = node->next;
  }
  return 0;
}

void hash_table_destroy(HashTable *ht) {
  if (!ht->buckets)
    return;

  for (size_t i = 0; i < ht->num_buckets; ++i) {
    HashNode *node = ht->buckets[i];
    while (node) {
      HashNode *tmp = node;
      node = node->next;
      mem_free(ht->alloc, tmp);
    }
  }
  mem_free(ht->alloc, ht->buckets);
  ht->buckets = NULL;
  ht->num_buckets = 0;
}