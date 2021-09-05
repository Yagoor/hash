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

#include "hash_table.h"
#include "hash_table_iterator.h"

/* Kept on uuids.c to make this file cleaner */
extern char uuids[1000][37];

typedef struct {
  uuid_t uuid;
} uuid_key_t;

typedef struct {
  uint32_t x;
} uuid_data_t;

#define UUID_HASH_ENTRIES_SIZE    5000

typedef struct {
  hash_table_t  hash_table;
  uint8_t       data[(sizeof(hash_table_entry_t) + sizeof(uuid_key_t) +
      sizeof(uuid_data_t)) * UUID_HASH_ENTRIES_SIZE];
} uuid_hash_table_t;

uint32_t uuid_hash_function(uint8_t *key)
{
  /* Hash djb2 */
  uint32_t hash = 5381;
  uuid_key_t *uuid_key;
  char uuid_str[37];
  char *str;
  char c;

  uuid_key = (uuid_key_t *)key;
  str = uuid_str;
  uuid_unparse_lower(uuid_key->uuid, uuid_str);

  while ((c = *str++))
  {
    hash = ((hash << 5) + hash) + c;
  }

  return (hash);
}


void test_uuid_hash(void **state)
{
  uuid_key_t uuid_key;
  uuid_data_t uuid_data;
  uuid_hash_table_t uuid_hash;

  uuid_hash = (*(uuid_hash_table_t *)(*state));

  /* Remove one item */
  uuid_parse(uuids[5], uuid_key.uuid);
  uuid_data.x = 5000;

  assert_true(hash_table_remove((hash_table_t *)&uuid_hash,
      (uint8_t *)&uuid_key,
      (uint8_t *)&uuid_data));
  assert_true(uuid_data.x == 5);

  /* Try to remove it again */
  uuid_parse(uuids[5], uuid_key.uuid);
  uuid_data.x = 5000;
  assert_false(hash_table_remove((hash_table_t *)&uuid_hash,
      (uint8_t *)&uuid_key,
      (uint8_t *)&uuid_data));
  assert_true(uuid_data.x == 5000);

  /* Try to remove item that was not added */
  uuid_parse("0e5e756a-0d72-11ec-82a8-0242ac130003", uuid_key.uuid);
  uuid_data.x = 4999;
  assert_false(hash_table_remove((hash_table_t *)&uuid_hash,
      (uint8_t *)&uuid_key,
      (uint8_t *)&uuid_data));
  assert_true(uuid_data.x == 4999);

  /* Get item from hash_table and check content */
  uuid_parse(uuids[10], uuid_key.uuid);
  assert_true(hash_table_get((hash_table_t *)&uuid_hash, (uint8_t *)&uuid_key,
      (uint8_t *)&uuid_data));
  assert_true(uuid_data.x == 10);

  /* Remove same item from hash_table */
  uuid_parse(uuids[10], uuid_key.uuid);
  assert_true(hash_table_remove((hash_table_t *)&uuid_hash,
      (uint8_t *)&uuid_key,
      NULL));

  /* Try to get it again */
  uuid_parse(uuids[10], uuid_key.uuid);
  assert_false(hash_table_get((hash_table_t *)&uuid_hash, (uint8_t *)&uuid_key,
      (uint8_t *)&uuid_data));
}


void test_uuid_hash_iterator(void **state)
{
  uint32_t i;
  uuid_t uuid;
  uuid_hash_table_t uuid_hash;
  hash_table_iterator_t hash_table_iterator;
  uuid_key_t uuid_key;
  uuid_data_t uuid_data;
  uint8_t data_checker[1000];

  uuid_hash = (*(uuid_hash_table_t *)(*state));

  memset(data_checker, 0, sizeof(data_checker));
  hash_table_iterator_init(&hash_table_iterator, (hash_table_t *)&uuid_hash);

  while (hash_table_iterator_get_next(&hash_table_iterator,
      (hash_table_t *)&uuid_hash, (uint8_t *)&uuid_key,
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
  uint32_t i;
  uuid_key_t uuid_key;
  uuid_data_t uuid_data;
  uuid_hash_table_t *uuid_hash;

  uuid_hash = malloc(sizeof(uuid_hash_table_t));

  if (uuid_hash == NULL) {
    return (-1);
  }

  /* Initialize hash_table */
  assert_true(hash_table_init((hash_table_t *)uuid_hash, uuid_hash_function,
      UUID_HASH_ENTRIES_SIZE, sizeof(uuid_data_t), sizeof(uuid_key_t)));

  /* Populate hash_table ensuring that repeated keys is not allowed */
  for (i = 1; i <= sizeof(uuids) / sizeof(char[37]); i++)
  {
    uuid_parse(uuids[i - 1], uuid_key.uuid);
    uuid_data.x = i - 1;

    assert_true(hash_table_insert((hash_table_t *)uuid_hash,
        (uint8_t *)&uuid_key,
        (uint8_t *)&uuid_data));

    /* Check hash_table count */
    assert_true(hash_table_count((hash_table_t *)uuid_hash) == i);

    assert_false(hash_table_insert((hash_table_t *)uuid_hash,
        (uint8_t *)&uuid_key,
        (uint8_t *)&uuid_data));

    /* Check hash_table count */
    assert_true(hash_table_count((hash_table_t *)uuid_hash) == i);
  }

  *state = uuid_hash;

  return (0);
}


int teardown(void **state)
{
  uuid_hash_table_t *uuid_hash;

  uuid_hash = *state;

  free(uuid_hash);

  return (0);
}


int main(void)
{
  const struct CMUnitTest tests[] =
  {
    cmocka_unit_test(test_uuid_hash),
    cmocka_unit_test(test_uuid_hash_iterator),
  };

  cmocka_set_message_output(CM_OUTPUT_XML);

  int count_fail_tests = cmocka_run_group_tests(tests, setup, teardown);

  return (count_fail_tests);
}
