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
 * @file hash_table.c
 *
 * @author Yago Fontoura do Rosario <yago.rosario@hotmail.com.br>
 */

#include <string.h>

#include "hash_table.h"

/**
 * @brief Function to increment the data size with the hash_table_table entry size
 *
 * @param data_size Data size
 * @return uint32_t Data size + hash_table entry size
 */
static inline uint32_t hash_entry_size(hash_table_t *hash_table)
__attribute__((always_inline));

static inline uint8_t *hash_entry_key(hash_table_t *hash_table,
    hash_entry_t *hash_entry)
__attribute__((always_inline));

static inline uint8_t *hash_entry_data(hash_table_t *hash_table,
    hash_entry_t *hash_entry)
__attribute__((always_inline));

/**
 * @brief Function to increment the data size with the hash_table_table entry size
 *
 * @param data_size Data size
 * @return uint32_t Data size + hash_table entry size
 */
static inline uint32_t hash_entry_size(hash_table_t *hash_table)
{
  return (sizeof(hash_entry_t) + hash_table->key_size + hash_table->data_size);
}


static inline uint8_t *hash_entry_key(hash_table_t *hash_table,
    hash_entry_t *hash_entry)
{
  (void)hash_table;
  return (hash_entry->key);
}


static inline uint8_t *hash_entry_data(hash_table_t *hash_table,
    hash_entry_t *hash_entry)
{
  return (hash_entry->data + hash_table->key_size);
}


/**
 * @brief Function to find an entry in the hash_table
 *
 * @param hash_table Hash pointer
 * @param key Key
 * @return hash_entry_t* A pointer to the hash_table_table entry found or NULL if not found
 */
static hash_entry_t *hash_find(hash_table_t *hash_table, uint8_t *key)
{
  uint32_t i;
  uint32_t index;
  uint32_t position;
  hash_entry_t *hash_entry;

  /* Convert hash_table key to index */
  index = hash_table->hash_function(key) % hash_table->size;

  /* Iterate over the entries looking for an empty one starting from the index */
  for (i = 0; i < hash_table->size; i++)
  {
    position = index * hash_entry_size(hash_table);
    hash_entry = (hash_entry_t *)(hash_table->entries + position);

    if (!hash_entry->used) {
      return (hash_entry);
    } else {
      /* If entry is used by the same key, clear it and break. */
      if (!memcmp(hash_entry->key, key, hash_table->key_size)) {
        return (hash_entry);
      }
    }

    index++;
    index %= hash_table->size;
  }

  return (NULL);
}


uint8_t hash_init(hash_table_t *hash_table, hash_function_t hash_function,
    uint32_t size, uint32_t data_size, uint32_t key_size)
{
  hash_table->size = size;
  hash_table->count = 0;
  hash_table->data_size = data_size;
  hash_table->key_size = key_size;
  hash_table->hash_function = hash_function;
  memset(hash_table->entries, 0, hash_entry_size(hash_table) * size);

  return (1);
}


uint8_t hash_insert(hash_table_t *hash_table, uint8_t *key, uint8_t *data)
{
  hash_entry_t *hash_entry;

  hash_entry = hash_find(hash_table, key);

  /* Set data and mark as used if empty entry is available */
  if (hash_entry && !hash_entry->used) {
    hash_table->count++;
    hash_entry->used = 1;
    /* Save key and data */
    memcpy(hash_entry_key(hash_table, hash_entry), key, hash_table->key_size);
    memcpy(hash_entry_data(hash_table, hash_entry), data,
        hash_table->data_size);
    return (1);
  } else {
    return (0);
  }
}


uint8_t hash_remove(hash_table_t *hash_table, uint8_t *key, uint8_t *data)
{
  hash_entry_t *hash_entry;

  hash_entry = hash_find(hash_table, key);

  /* Clear data and mark as not used if entry is found */
  if (hash_entry && hash_entry->used) {
    hash_table->count--;
    hash_entry->used = 0;
    if (data) {
      /* Copy data */
      memcpy(data, hash_entry_data(hash_table,
          hash_entry), hash_table->data_size);
    }
    memset(hash_entry_key(hash_table, hash_entry), 0, hash_table->key_size);
    memset(hash_entry_data(hash_table, hash_entry), 0, hash_table->data_size);

    return (1);
  } else {
    return (0);
  }
}


uint8_t hash_get(hash_table_t *hash_table, uint8_t *key, uint8_t *data)
{
  hash_entry_t *hash_entry;

  hash_entry = hash_find(hash_table, key);

  /* If entry is found and it is used */
  if (hash_entry && hash_entry->used) {
    /* Copy data */
    memcpy(data, hash_entry_data(hash_table, hash_entry),
        hash_table->data_size);
    return (1);
  } else {
    return (0);
  }
}
