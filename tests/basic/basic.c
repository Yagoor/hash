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

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
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

static uint64_t basic_hash_function(const uint8_t *key, const uint32_t size)
{
  uint64_t hash = 0;
  uint32_t i;

  for (i = 0; i < size; i++)
  {
    hash = hash + (key[i] * 32);
  }

  return (hash);
}


void test_basic_hash(void **state)
{
  basic_ht_t basic_hash;
  basic_key_t basic_key;
  basic_data_t basic_data;

  basic_hash = (*(basic_ht_t *)(*state));

  /* Try to insert when hash_table is full */
  basic_key.key = 20;
  assert_false(ht_insert((ht_t *)&basic_hash,
      (uint8_t *)&basic_key,
      (uint8_t *)&basic_data));

  /* Remove one item */
  basic_key.key = 9;
  assert_true(ht_remove((ht_t *)&basic_hash,
      (uint8_t *)&basic_key,
      (uint8_t *)&basic_data));
  assert_true(basic_data.x == 9);
  assert_true(basic_data.y == 1);

  /* Try to remove it again */
  basic_key.key = 9;
  assert_false(ht_remove((ht_t *)&basic_hash,
      (uint8_t *)&basic_key,
      (uint8_t *)&basic_data));

  /* Try to remove item that was not added */
  basic_key.key = 20;
  assert_false(ht_remove((ht_t *)&basic_hash,
      (uint8_t *)&basic_key,
      (uint8_t *)&basic_data));

  /* Get item from hash_table and check content */
  basic_key.key = 4;
  assert_true(ht_get((ht_t *)&basic_hash, (uint8_t *)&basic_key,
      (uint8_t *)&basic_data));
  assert_true(basic_data.x == 4);
  assert_true(basic_data.y == 6);

  /* Remove same item from hash_table */
  basic_key.key = 4;
  assert_true(ht_remove((ht_t *)&basic_hash,
      (uint8_t *)&basic_key,
      NULL));

  /* Try to get it again */
  basic_key.key = 4;
  assert_false(ht_get((ht_t *)&basic_hash,
      (uint8_t *)&basic_key,
      (uint8_t *)&basic_data));
}


void test_basic_hash_iterator(void **state)
{
  uint32_t i;
  basic_ht_t basic_hash;
  ht_iter_t ht_iterator;
  basic_key_t basic_key;
  basic_data_t basic_data;
  uint8_t key_checker[BASIC_HASH_ENTRIES_SIZE];

  basic_hash = (*(basic_ht_t *)(*state));

  memset(key_checker, 0, sizeof(key_checker));
  ht_iter_init(&ht_iterator, (ht_t *)&basic_hash);

  while (ht_iter_get_next(&ht_iterator,
      (ht_t *)&basic_hash, (uint8_t *)&basic_key,
      (uint8_t *)&basic_data))
  {
    assert_true(key_checker[basic_key.key - 1] == 0);
    assert_true(basic_data.x + basic_data.y == 10);
    key_checker[basic_key.key - 1] = 1;
  }

  /* Verify key checker */
  for (i = 0; i < BASIC_HASH_ENTRIES_SIZE; i++)
  {
    assert_true(key_checker[i] == 1);
  }
}


int setup(void **state)
{
  uint8_t i;
  basic_key_t basic_key;
  basic_data_t basic_data;
  basic_ht_t *basic_hash;

  basic_hash = malloc(sizeof(basic_ht_t));

  if (basic_hash == NULL) {
    return (-1);
  }

  /* Initialize hash_table */
  assert_true(ht_init((ht_t *)basic_hash, basic_hash_function,
      BASIC_HASH_ENTRIES_SIZE, sizeof(basic_data_t), sizeof(basic_key_t)));

  /* Populate hash_table ensuring that repeated keys is not allowed */
  for (i = 1; i <= BASIC_HASH_ENTRIES_SIZE; i++)
  {
    basic_key.key = i;
    basic_data.x = i;
    basic_data.y = BASIC_HASH_ENTRIES_SIZE - i;
    assert_true(ht_insert((ht_t *)basic_hash,
        (uint8_t *)&basic_key,
        (uint8_t *)&basic_data));

    /* Check hash_table count */
    assert_true(ht_count((ht_t *)basic_hash) == i);

    assert_false(ht_insert((ht_t *)basic_hash,
        (uint8_t *)&basic_key,
        (uint8_t *)&basic_data));

    /* Check hash_table count */
    assert_true(ht_count((ht_t *)basic_hash) == i);
  }

  *state = basic_hash;

  return (0);
}


int teardown(void **state)
{
  basic_ht_t *basic_hash;

  basic_hash = *state;

  free(basic_hash);

  return (0);
}


int main(void)
{
  const struct CMUnitTest tests[] =
  {
    cmocka_unit_test(test_basic_hash),
    cmocka_unit_test(test_basic_hash_iterator),
  };

  cmocka_set_message_output(CM_OUTPUT_XML);

  int count_fail_tests = cmocka_run_group_tests(tests, setup, teardown);

  return (count_fail_tests);
}
