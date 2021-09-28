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
 * @file ht.h
 *
 * @author Yago Fontoura do Rosario <yago.rosario@hotmail.com.br>
 */

#ifndef HASH_H
#define HASH_H

#include <stdint.h>

/**
 * @brief Hash entry struct
 *
 */
typedef struct {
  /**
   * @brief Indicates if entry is used
   *
   */
  uint8_t used : 1;
} ht_entry_t;

/**
 * @brief Function callback
 *
 */
typedef uint32_t (*hash_function_t) (uint8_t *key);

/**
 * @brief Hash struct
 *
 */
typedef struct {
  /**
   * @brief Hash size
   *
   */
  uint32_t              size;

  /**
   * @brief Number of used entries
   *
   */
  uint32_t              count;

  /**
   * @brief Hash data size
   *
   */
  uint32_t              data_size;

  /**
   * @brief Hash data size
   *
   */
  uint32_t              key_size;

  /**
   * @brief Hash function callback
   *
   */
  hash_function_t       hash_function;
} ht_t;

/**
 * @brief Function to initialize a hash_table
 *
 * @param[in] hash_table Hash pointer
 * @param[in] hash_function Hash function callback
 * @param[in] size Hash size
 * @param[in] data_size Hash data size
 * @param[in] key_size Hash key size
 * @return uint8_t 1 if the hash_table was initialized else 0
 */
uint8_t ht_init(ht_t *hash_table, hash_function_t hash_function,
    uint32_t size, uint32_t data_size, uint32_t key_size);

/**
 * @brief Function to insert an item in the hash_table
 *
 * @param[in] hash_table Hash pointer
 * @param[in] key Item key
 * @param[in] data Item data
 * @return uint8_t 1 if the item was inserted else 0
 */
uint8_t ht_insert(ht_t *hash_table, uint8_t *key,
    uint8_t *data);

/**
 * @brief Function to remove an item from the hash_table
 *
 * @param[in] hash_table Hash pointer
 * @param[in] key Itek key
 * @param[out] data Item data
 * @return uint8_t 1 if the item was removed else 0
 */
uint8_t ht_remove(ht_t *hash_table, uint8_t *key,
    uint8_t *data);

/**
 * @brief Function to get an item from the hash_table
 *
 * @param[in] hash_table Hash pointer
 * @param[in] key Item key
 * @param[out] data Item data
 * @return uint8_t 1 if the item was found else 0
 */
uint8_t ht_get(ht_t *hash_table, uint8_t *key, uint8_t *data);

/**
 * @brief Function to get the number of used entries in the hash_table
 *
 * @param[in] hash_table Hash pointer
 * @return uint32_t Number of itens in the hash_table
 */
static inline uint32_t ht_count(ht_t *hash_table)
__attribute__((always_inline));

#include "ht_inline.h"

#endif /* HASH_H */
