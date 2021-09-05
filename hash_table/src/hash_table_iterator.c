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

#include "hash_table_iterator.h"

uint8_t hash_table_iterator_init(hash_table_iterator_t *hash_table_iterator,
    hash_table_t *hash_table)
{
  hash_table_iterator->current = 0;
  hash_table_iterator->hash_table = hash_table;

  return (1);
}


uint8_t hash_table_iterator_get_next(hash_table_iterator_t *hash_table_iterator,
    hash_table_t *hash_table, uint8_t *key, uint8_t *data)
{
  uint32_t position;
  uint8_t *entries;
  hash_table_entry_t *hash_entry;
  uint8_t *hash_entry_key;
  uint8_t *hash_entry_data;

  entries = (uint8_t *)hash_table + sizeof(hash_table_t);

  /* Iterate over the entries */
  for ( ; hash_table_iterator->current < hash_table->size;
      hash_table_iterator->current++)
  {
    position = hash_table_iterator->current *
        (sizeof(hash_table_entry_t) + hash_table->key_size +
        hash_table->data_size);
    hash_entry = (hash_table_entry_t *)(entries + position);

    if (hash_entry->used) {
      hash_entry_key = (uint8_t *)(hash_entry + sizeof(hash_table_entry_t));
      hash_entry_data = (uint8_t *)(hash_entry_key + hash_table->key_size);

      memcpy(key, hash_entry_key, hash_table->key_size);
      memcpy(data, hash_entry_data, hash_table->data_size);
      hash_table_iterator->current++;
      return (1);
    }
  }

  return (0);
}
