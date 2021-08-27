/*
 * MIT License
 *
 * Copyright (c) 2021 Yago Fontoura do Rosário <yago.rosario@hotmail.com.br>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

/**
 * @file hash.c
 *
 * @author Yago Fontoura do Rosario <yago.rosario@hotmail.com.br>
 */

#include "hash.h"

#include <string.h>

/**
 * @brief Function to increment the data size with the hash entry size
 *
 * @param data_size Data size
 * @return uint32_t Data size + hash entry size
 */
static inline uint32_t hash_entry_size(uint32_t data_size)
__attribute__((always_inline));

/**
 * @brief Function to increment the data size with the hash entry size
 *
 * @param data_size Data size
 * @return uint32_t Data size + hash entry size
 */
static inline uint32_t hash_entry_size(uint32_t data_size)
{
  return (data_size + sizeof(hash_entry_t));
}


/**
 * @brief Function to find an entry in the hash
 *
 * @param hash Hash pointer
 * @param key Key
 * @return hash_entry_t* A pointer to the hash entry found or NULL if not found
 */
static hash_entry_t *hash_find(hash_t *hash, uint32_t key)
{
  uint32_t i;
  uint32_t index;
  uint32_t position;
  hash_entry_t *hash_entry;

  /* Convert hash key to index */
  index = hash->hash_function(key) % hash->size;

  /* Iterate over the entries looking for an empty one starting from the index */
  for (i = 0; i < hash->size; i++)
  {
    position = index * hash_entry_size(hash->data_size);
    hash_entry = (hash_entry_t *)(&hash->entries[position]);

    if (!hash_entry->used) {
      return (hash_entry);
    } else {
      /* If entry is used by the same key, clear it and break. */
      if (hash_entry->key == key) {
        return (hash_entry);
      }
    }

    index++;
    index %= hash->size;
  }

  return (NULL);
}


uint8_t hash_init(hash_t *hash, hash_function_t hash_function, uint32_t size,
    uint32_t data_size)
{
  hash->size = size;
  hash->count = 0;
  hash->data_size = data_size;
  hash->hash_function = hash_function;
  memset(hash->entries, 0, hash_entry_size(data_size) * size);

  return (1);
}


uint8_t hash_insert(hash_t *hash, uint32_t key, uint8_t *data)
{
  hash_entry_t *empty_entry;

  empty_entry = hash_find(hash, key);

  /* Set data and mark as used if empty entry is available */
  if (empty_entry && !empty_entry->used) {
    hash->count++;
    empty_entry->used = 1;
    /* Save key */
    empty_entry->key = key;
    memcpy(empty_entry->data, data, hash->data_size);
    return (1);
  } else {
    return (0);
  }
}


uint8_t hash_remove(hash_t *hash, uint32_t key)
{
  hash_entry_t *hash_entry;

  hash_entry = hash_find(hash, key);

  /* Clear data and mark as not used if entry is found */
  if (hash_entry && hash_entry->used) {
    hash->count--;
    hash_entry->used = 0;
    memset(hash_entry->data, 0, hash->data_size);
    return (1);
  } else {
    return (0);
  }
}


uint8_t hash_get(hash_t *hash, uint32_t key, uint8_t *data)
{
  hash_entry_t *hash_entry;

  hash_entry = hash_find(hash, key);

  /* If entry is found and it is used */
  if (hash_entry && hash_entry->used) {
    /* Copy data */
    memcpy(data, hash_entry->data, hash->data_size);
    return (1);
  } else {
    return (0);
  }
}
