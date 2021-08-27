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

#include "hash_table.h"

typedef struct {
  uint32_t      x;
  uint32_t      y;
} basic_data_t;

typedef struct {
  hash_entry_t  entry;
  basic_data_t  data;
} basic_hash_entry_t;

#define BASIC_HASH_ENTRIES_SIZE    10

typedef struct {
  hash_table_t          hash_table;
  basic_hash_entry_t    data[BASIC_HASH_ENTRIES_SIZE];
} basic_hash_table_t;

uint32_t basic_hash_function(uint32_t key)
{
  return ((key * 32) >> 8);
}


void null_test_success(void **state)
{
  uint8_t i;
  basic_hash_table_t basic_hash;
  basic_data_t basic_data;

  /* Initialize hash_table */
  assert_true(hash_init((hash_table_t *)&basic_hash,
      (hash_function_t)basic_hash_function,
      BASIC_HASH_ENTRIES_SIZE, sizeof(basic_data_t)));

  /* Populate hash_table ensuring that repeated keys is not allowed */
  for (i = 1; i <= BASIC_HASH_ENTRIES_SIZE; i++)
  {
    basic_data.x = i;
    basic_data.y = BASIC_HASH_ENTRIES_SIZE - i;
    assert_true(hash_insert((hash_table_t *)&basic_hash, i,
        (uint8_t *)&basic_data));

    /* Check hash_table count */
    assert_true(hash_count((hash_table_t *)&basic_hash) == i);

    assert_false(hash_insert((hash_table_t *)&basic_hash, i,
        (uint8_t *)&basic_data));

    /* Check hash_table count */
    assert_true(hash_count((hash_table_t *)&basic_hash) == i);
  }

  /* Try to insert when hash_table is full */
  assert_false(hash_insert((hash_table_t *)&basic_hash, 20,
      (uint8_t *)&basic_data));

  /* Remove one item */
  assert_true(hash_remove((hash_table_t *)&basic_hash, 9));

  /* Try to remove it again */
  assert_false(hash_remove((hash_table_t *)&basic_hash, 9));

  /* Try to remove item that was not added */
  assert_false(hash_remove((hash_table_t *)&basic_hash, 20));

  /* Get item from hash_table and check content */
  assert_true(hash_get((hash_table_t *)&basic_hash, 4, (uint8_t *)&basic_data));
  assert_true(basic_data.x == 4);
  assert_true(basic_data.y == 6);

  /* Remove same item from hash_table */
  assert_true(hash_remove((hash_table_t *)&basic_hash, 4));

  /* Try to get it again */
  assert_false(hash_get((hash_table_t *)&basic_hash, 4,
      (uint8_t *)&basic_data));
}


int setup(void **state)
{
  return (0);
}


int teardown(void **state)
{
  return (0);
}


int main(void)
{
  const struct CMUnitTest tests[] =
  {
    cmocka_unit_test(null_test_success),
  };

  cmocka_set_message_output(CM_OUTPUT_XML);

  int count_fail_tests = cmocka_run_group_tests(tests, setup, teardown);

  return (count_fail_tests);
}
