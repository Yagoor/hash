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
#include <uuid/uuid.h>

#include "ht.h"
#include "ht_iter.h"
#include "ht_fn_djb.h"

/* Kept on uuids.c to make this file cleaner */
extern char uuids[1000][37];

typedef struct {
  uuid_t uuid;
} uuid_key_t;

typedef struct {
  uint32_t x;
} uuid_data_t;

#define UUID_HASH_ENTRIES_SIZE    5000

static ht_t hash_table;
static uint8_t hash_table_data[(sizeof(ht_entry_t) + sizeof(uuid_key_t) +
    sizeof(uuid_data_t)) * UUID_HASH_ENTRIES_SIZE];

void test_hash(void **state)
{
  (void)state;

  uuid_key_t uuid_key;
  uuid_data_t uuid_data;

  /* Remove one item */
  uuid_parse(uuids[5], uuid_key.uuid);
  uuid_data.x = 5000;

  assert_true(ht_remove(&hash_table,
      (uint8_t *)&uuid_key,
      (uint8_t *)&uuid_data));
  assert_true(uuid_data.x == 5);

  /* Try to remove it again */
  uuid_parse(uuids[5], uuid_key.uuid);
  uuid_data.x = 5000;
  assert_false(ht_remove(&hash_table,
      (uint8_t *)&uuid_key,
      (uint8_t *)&uuid_data));
  assert_true(uuid_data.x == 5000);

  /* Try to remove item that was not added */
  uuid_parse("0e5e756a-0d72-11ec-82a8-0242ac130003", uuid_key.uuid);
  uuid_data.x = 4999;
  assert_false(ht_remove(&hash_table,
      (uint8_t *)&uuid_key,
      (uint8_t *)&uuid_data));
  assert_true(uuid_data.x == 4999);

  /* Get item from hash_table and check content */
  uuid_parse(uuids[10], uuid_key.uuid);
  assert_true(ht_get(&hash_table, (uint8_t *)&uuid_key,
      (uint8_t *)&uuid_data));
  assert_true(uuid_data.x == 10);

  /* Remove same item from hash_table */
  uuid_parse(uuids[10], uuid_key.uuid);
  assert_true(ht_remove(&hash_table,
      (uint8_t *)&uuid_key,
      NULL));

  /* Try to get it again */
  uuid_parse(uuids[10], uuid_key.uuid);
  assert_false(ht_get(&hash_table, (uint8_t *)&uuid_key,
      (uint8_t *)&uuid_data));
}


void test_hash_iterator(void **state)
{
  (void)state;

  uint32_t i;
  uuid_t uuid;
  ht_iter_t ht_iterator;
  uuid_key_t uuid_key;
  uuid_data_t uuid_data;
  uint8_t data_checker[1000];

  memset(data_checker, 0, sizeof(data_checker));
  ht_iter_init(&ht_iterator, &hash_table);

  while (ht_iter_get_next(&ht_iterator,
      &hash_table, (uint8_t *)&uuid_key,
      (uint8_t *)&uuid_data))
  {
    /* Use lookup table to ensure data is unique based on setup */
    assert_true(data_checker[uuid_data.x] == 0);
    data_checker[uuid_data.x] = 1;

    uuid_parse(uuids[uuid_data.x], uuid);
    assert_true(uuid_compare(uuid, uuid_key.uuid) == 0);
  }

  /* Verify data checker */
  for (i = 0; i < 1000; i++)
  {
    assert_true(data_checker[i] == 1);
  }
}


int setup(void **state)
{
  (void)state;

  uint32_t i;
  uuid_key_t uuid_key;
  uuid_data_t uuid_data;

  memset(&hash_table, 0, sizeof(hash_table));
  memset(hash_table_data, 0, sizeof(hash_table_data));

  /* Initialize hash_table */
  assert_true(ht_init(&hash_table, ht_fn_djb, UUID_HASH_ENTRIES_SIZE,
      sizeof(uuid_data_t), sizeof(uuid_key_t), hash_table_data));

  /* Populate hash_table ensuring that repeated keys is not allowed */
  for (i = 1; i <= sizeof(uuids) / sizeof(char[37]); i++)
  {
    uuid_parse(uuids[i - 1], uuid_key.uuid);
    uuid_data.x = i - 1;

    assert_true(ht_insert(&hash_table,
        (uint8_t *)&uuid_key,
        (uint8_t *)&uuid_data));

    /* Check hash_table count */
    assert_true(ht_count(&hash_table) == i);

    assert_false(ht_insert(&hash_table,
        (uint8_t *)&uuid_key,
        (uint8_t *)&uuid_data));

    /* Check hash_table count */
    assert_true(ht_count(&hash_table) == i);
  }

  return (0);
}


int teardown(void **state)
{
  (void)state;

  return (0);
}


int group_setup(void **state)
{
  (void)state;

  return (0);
}


int group_teardown(void **state)
{
  (void)state;

  return (0);
}


int main(void)
{
  const struct CMUnitTest tests[] =
  {
    cmocka_unit_test_setup_teardown(test_hash,          setup,
        teardown),
    cmocka_unit_test_setup_teardown(test_hash_iterator, setup,
        teardown),
  };

  cmocka_set_message_output(CM_OUTPUT_XML);

  int count_fail_tests = cmocka_run_group_tests(tests, group_setup,
          group_teardown);

  return (count_fail_tests);
}
