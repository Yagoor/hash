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

#include <string.h>

#include "ht.h"
#include "ht_iter.h"

typedef struct {
  uint32_t key;
} basic_key_t;

typedef struct {
  uint32_t      x;
  uint32_t      y;
} basic_data_t;

#define BASIC_HASH_ENTRIES_SIZE    10

typedef struct {
  ht_t          hash_table;
  uint8_t       data[(sizeof(ht_entry_t) + sizeof(basic_key_t) +
      sizeof(basic_data_t)) * BASIC_HASH_ENTRIES_SIZE];
} basic_ht_t;

uint32_t ht_function(uint8_t *key)
{
  basic_key_t *basic_key;

  basic_key = (basic_key_t *)key;

  return ((basic_key->key * 32) >> 8);
}


int main(void)
{
  uint32_t i;
  basic_key_t basic_key;
  basic_data_t basic_data;
  basic_ht_t basic_hash;
  ht_iter_t ht_iterator;
  uint8_t key_checker[BASIC_HASH_ENTRIES_SIZE];

  ht_init((ht_t *)&basic_hash,
      (hash_function_t)ht_function,
      BASIC_HASH_ENTRIES_SIZE, sizeof(basic_data_t), sizeof(basic_key_t));

  /* Populate hash_table ensuring that repeated keys is not allowed */
  for (i = 1; i <= BASIC_HASH_ENTRIES_SIZE; i++)
  {
    basic_key.key = i;
    basic_data.x = i;
    basic_data.y = BASIC_HASH_ENTRIES_SIZE - i;
    if (!(ht_insert((ht_t *)&basic_hash,
        (uint8_t *)&basic_key,
        (uint8_t *)&basic_data))) {
      return (1);
    }

    /* Check hash_table count */
    if (!(ht_count((ht_t *)&basic_hash) == i)) {
      return (1);
    }
  }

  memset(key_checker, 0, sizeof(key_checker));
  ht_iter_init(&ht_iterator, (ht_t *)&basic_hash);

  while (ht_iter_get_next(&ht_iterator,
      (ht_t *)&basic_hash, (uint8_t *)&basic_key,
      (uint8_t *)&basic_data))
  {
    if (!(key_checker[basic_key.key - 1] == 0)) {
      return (1);
    }
    if (!(basic_data.x + basic_data.y == 10)) {
      return (1);
    }
    key_checker[basic_key.key - 1] = 1;
  }

  /* Verify key checker */
  for (i = 0; i < BASIC_HASH_ENTRIES_SIZE; i++)
  {
    if (!(key_checker[i] == 1)) {
      return (1);
    }
  }

  /* Try to insert when hash_table is full */
  basic_key.key = 20;
  if (ht_insert((ht_t *)&basic_hash,
      (uint8_t *)&basic_key,
      (uint8_t *)&basic_data)) {
    return (1);
  }

  /* Remove one item */
  basic_key.key = 9;
  if (!(ht_remove((ht_t *)&basic_hash,
      (uint8_t *)&basic_key,
      (uint8_t *)&basic_data))) {
    return (1);
  }
  if (!(basic_data.x == 9)) {
    return (1);
  }
  if (!(basic_data.y == 1)) {
    return (1);
  }

  /* Try to remove it again */
  basic_key.key = 9;
  if (ht_remove((ht_t *)&basic_hash,
      (uint8_t *)&basic_key,
      (uint8_t *)&basic_data)) {
    return (1);
  }

  /* Try to remove item that was not added */
  basic_key.key = 20;
  if (ht_remove((ht_t *)&basic_hash,
      (uint8_t *)&basic_key,
      (uint8_t *)&basic_data)) {
    return (1);
  }

  /* Get item from hash_table and check content */
  basic_key.key = 4;
  if (!(ht_get((ht_t *)&basic_hash, (uint8_t *)&basic_key,
      (uint8_t *)&basic_data))) {
    return (1);
  }
  if (!(basic_data.x == 4)) {
    return (1);
  }
  if (!(basic_data.y == 6)) {
    return (1);
  }

  /* Remove same item from hash_table */
  basic_key.key = 4;
  if (!(ht_remove((ht_t *)&basic_hash,
      (uint8_t *)&basic_key,
      NULL))) {
    return (1);
  }

  /* Try to get it again */
  basic_key.key = 4;
  if (ht_get((ht_t *)&basic_hash,
      (uint8_t *)&basic_key,
      (uint8_t *)&basic_data)) {
    return (1);
  }

  return (0);
}
