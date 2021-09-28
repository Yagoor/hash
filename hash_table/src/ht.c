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
 * @file ht.c
 *
 * @author Yago Fontoura do Rosario <yago.rosario@hotmail.com.br>
 */

#include <string.h>

#include "ht.h"


/**
 * @brief Function to find an entry in the hash_table
 *
 * @param hash_table Hash pointer
 * @param key Key
 * @return ht_entry_t* A pointer to the ht_iable entry found or NULL if not found
 */
static ht_entry_t *hash_find(ht_t *hash_table, uint8_t *key)
{
  uint32_t i;
  uint32_t index;
  uint32_t position;
  uint8_t *entries;
  uint8_t *hash_entry_key;
  ht_entry_t *hash_entry;

  entries = (uint8_t *)hash_table + sizeof(ht_t);
  /* Convert hash_table key to index */
  index = hash_table->hash_function(key) % hash_table->size;

  /* Iterate over the entries looking for an empty one starting from the index */
  for (i = 0; i < hash_table->size; i++)
  {
    position = index *
        (sizeof(ht_entry_t) + hash_table->key_size +
        hash_table->data_size);
    hash_entry = (ht_entry_t *)(entries + position);

    if (!hash_entry->used) {
      return (hash_entry);
    } else {
      hash_entry_key = (uint8_t *)(hash_entry + sizeof(ht_entry_t));

      /* If entry is used by the same key, clear it and break. */
      if (!memcmp(hash_entry_key, key, hash_table->key_size)) {
        return (hash_entry);
      }
    }

    index++;
    index %= hash_table->size;
  }

  return (NULL);
}


uint8_t ht_init(ht_t *hash_table, hash_function_t hash_function,
    uint32_t size, uint32_t data_size, uint32_t key_size)
{
  uint8_t *entries;

  entries = (uint8_t *)hash_table + sizeof(ht_t);

  hash_table->size = size;
  hash_table->count = 0;
  hash_table->data_size = data_size;
  hash_table->key_size = key_size;
  hash_table->hash_function = hash_function;
  memset(entries, 0,
      (sizeof(ht_entry_t) + key_size + data_size) * size);

  return (1);
}


uint8_t ht_insert(ht_t *hash_table, uint8_t *key, uint8_t *data)
{
  ht_entry_t *hash_entry;
  uint8_t *hash_entry_key;
  uint8_t *hash_entry_data;

  hash_entry = hash_find(hash_table, key);

  /* Set data and mark as used if empty entry is available */
  if (hash_entry && !hash_entry->used) {
    hash_entry_key = (uint8_t *)(hash_entry + sizeof(ht_entry_t));
    hash_entry_data = (uint8_t *)(hash_entry_key + hash_table->key_size);

    hash_table->count++;
    hash_entry->used = 1;
    /* Save key and data */
    memcpy(hash_entry_key, key, hash_table->key_size);
    memcpy(hash_entry_data, data, hash_table->data_size);
    return (1);
  } else {
    return (0);
  }
}


uint8_t ht_remove(ht_t *hash_table, uint8_t *key, uint8_t *data)
{
  ht_entry_t *hash_entry;
  uint8_t *hash_entry_key;
  uint8_t *hash_entry_data;

  hash_entry = hash_find(hash_table, key);

  /* Clear data and mark as not used if entry is found */
  if (hash_entry && hash_entry->used) {
    hash_entry_key = (uint8_t *)(hash_entry + sizeof(ht_entry_t));
    hash_entry_data = (uint8_t *)(hash_entry_key + hash_table->key_size);

    hash_table->count--;
    hash_entry->used = 0;
    if (data) {
      /* Copy data */
      memcpy(data, hash_entry_data, hash_table->data_size);
    }
    memset(hash_entry_key, 0, hash_table->key_size);
    memset(hash_entry_data, 0, hash_table->data_size);

    return (1);
  } else {
    return (0);
  }
}


uint8_t ht_get(ht_t *hash_table, uint8_t *key, uint8_t *data)
{
  ht_entry_t *hash_entry;
  uint8_t *hash_entry_data;

  hash_entry = hash_find(hash_table, key);

  /* If entry is found and it is used */
  if (hash_entry && hash_entry->used) {
    /* Copy data */
    hash_entry_data =
        (uint8_t *)(hash_entry + sizeof(ht_entry_t) +
        hash_table->key_size);

    memcpy(data, hash_entry_data, hash_table->data_size);
    return (1);
  } else {
    return (0);
  }
}
