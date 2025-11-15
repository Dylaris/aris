/*
cook.h - v0.05 - Dylaris 2025
===================================================

BRIEF:
  Small C containers (vector and hash table) implemented using C99.

NOTICE:
  Not compatible with C++.

USAGE:
  In exactly one source file, define the implementation macro
  before including this header:
  ```
    #define COOK_IMPLEMENTATION
    #include "cook.h"
  ```
  In other files, just include the header without the macro.

HISTORY:
    v0.05 Remove 'list', 'deque', 'string', 'file'
    v0.04 Support data-structure 'mini hash table'
    v0.03 Support data-structure 'doubly instrusive linked list'
    v0.02 Support data-structure 'deque'

LICENSE:
  See the end of this file for further details.
*/

#ifndef COOK_H
#define COOK_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

/*
 * dynamic array
 */
typedef struct Cook_Vector_Header {
    size_t size;
    size_t capacity;
} Cook_Vector_Header;

#define cook_vec_header(vec)   ((Cook_Vector_Header*)((char*)(vec) - sizeof(Cook_Vector_Header)))
#define cook_vec_size(vec)     ((vec) ? cook_vec_header(vec)->size : 0)
#define cook_vec_capacity(vec) ((vec) ? cook_vec_header(vec)->capacity : 0)
#define cook_vec_push(vec, item)                               \
    do {                                                       \
        if (cook_vec_size(vec) + 1 > cook_vec_capacity(vec)) { \
            (vec) = cook_vec_grow(vec, sizeof(*vec));          \
        }                                                      \
        (vec)[cook_vec_header(vec)->size++] = (item);          \
    } while (0)
#define cook_vec_pop(vec) ((vec)[--cook_vec_header(vec)->size])
#define cook_vec_isempty(vec) ((vec) ? cook_vec_size(vec) == 0 : false)
#define cook_vec_free(vec)                   \
    do {                                     \
        if (vec) free(cook_vec_header(vec)); \
        (vec) = NULL;                        \
    } while (0)
#define cook_vec_reset(vec)                      \
    do {                                         \
        if (vec) cook_vec_header(vec)->size = 0; \
    } while (0)
#define cook_vec_foreach(type, vec, iter) for (type *iter = (vec); iter < (vec)+cook_vec_size(vec); iter++)
#define cook_vec_swap_delete(vec, deleted_index)          \
    do {                                                  \
        if (cook_vec_isempty(vec)) break;                 \
        if ((deleted_index) >= cook_vec_size(vec)) break; \
        (vec)[deleted_index] = cook_vec_pop(vec);         \
    } while (0)
void *cook_vec_grow(void *vec, size_t item_size);


/*
 * static array
 */
#define cook_arr_len(arr) (sizeof(arr)/sizeof(arr[0]))
#define cook_arr_foreach(type, arr, iter) for (type *iter = (arr); iter < (arr)+cook_arr_len(arr); iter++)


/*
 * mini hash table
 *
 * A simple hash table that uses hash values as keys and external array indices as values.
 *
 * Features:
 * - Can use integers directly as keys/values, no external array needed
 * - Cannot handle hash collisions (different keys producing same hash)
 * - Low collision probability with good hash function or small datasets
 *
 * Designed for simplicity and small size, not safety.
 */
typedef struct Cook_Mini_Hash_Bucket {
    uint32_t key;
    uint32_t val;
} Cook_Mini_Hash_Bucket;

typedef struct Cook_Mini_Hash {
    uint32_t count;
    uint32_t capacity;
    Cook_Mini_Hash_Bucket *buckets;
} Cook_Mini_Hash;

#define COOK_MINI_HASH_MAX_LOAD 0.75

uint32_t fnv_hash(const char *str, size_t length);
bool cook_hash_set(Cook_Mini_Hash *lookup, uint32_t key, uint32_t val);
bool cook_hash_get(Cook_Mini_Hash *lookup, uint32_t key, uint32_t *out_val);
void cook_hash_free(Cook_Mini_Hash *lookup);

#endif /* COOK_H */

#ifdef COOK_IMPLEMENTATION

void *cook_vec_grow(void *vec, size_t item_size)
{
    size_t new_capacity, alloc_size;
    Cook_Vector_Header *new_header;

    new_capacity = cook_vec_capacity(vec) < 16
                   ? 16 : 2 * cook_vec_capacity(vec);
    alloc_size = sizeof(Cook_Vector_Header) + new_capacity*item_size;

    if (vec) {
        new_header = realloc(cook_vec_header(vec), alloc_size);
        assert(new_header != NULL && "out of memory");
    } else {
        new_header = malloc(alloc_size);
        assert(new_header != NULL && "out of memory");
        new_header->size = 0;
    }
    new_header->capacity = new_capacity;

    return (void*)((char*)new_header + sizeof(Cook_Vector_Header));
}

static Cook_Mini_Hash_Bucket *cook_hash__find_bucket(Cook_Mini_Hash_Bucket *buckets, uint32_t capacity, uint32_t key)
{
    uint32_t index = key % capacity;
    uint32_t start_index = index;

    for (;;) {
        if (buckets[index].key == key || buckets[index].key == 0) return &buckets[index];
        index = (index + 1) % capacity;
        if (index == start_index) return NULL;
    }
}

static bool cook_hash__expand(Cook_Mini_Hash *lookup, size_t new_capacity)
{
    if (new_capacity <= lookup->capacity) return false;

    Cook_Mini_Hash_Bucket *new_buckets = malloc(sizeof(Cook_Mini_Hash_Bucket)*new_capacity);
    if (!new_buckets) return false;
    for (uint32_t i = 0; i < new_capacity; i++) new_buckets[i].key = 0;

    for (uint32_t i = 0; i < lookup->capacity; i++) {
        Cook_Mini_Hash_Bucket *bucket = &lookup->buckets[i];
        if (bucket->key == 0) continue;
        Cook_Mini_Hash_Bucket *dest = cook_hash__find_bucket(new_buckets, new_capacity, bucket->key);
        dest->key = bucket->key;
        dest->val = bucket->val;
    }

    lookup->capacity = new_capacity;
    if (lookup->buckets) free(lookup->buckets);
    lookup->buckets = new_buckets;

    return true;
}

uint32_t fnv_hash(const char *str, size_t length)
{
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < length; i++) {
        hash ^= (uint8_t)str[i];
        hash *= 16777619;
    }
    return hash;
}

bool cook_hash_set(Cook_Mini_Hash *lookup, uint32_t key, uint32_t val)
{
    if (lookup->count + 1 > lookup->capacity * COOK_MINI_HASH_MAX_LOAD) {
        uint32_t new_capacity = lookup->capacity < 16 ? 16 : 2*lookup->capacity;
        if (!cook_hash__expand(lookup, new_capacity)) return false;
    }

    Cook_Mini_Hash_Bucket *bucket = cook_hash__find_bucket(lookup->buckets, lookup->capacity, key);
    if (!bucket) return false;

    bucket->key = key;
    bucket->val = val;
    lookup->count++;

    return true;
}

bool cook_hash_get(Cook_Mini_Hash *lookup, uint32_t key, uint32_t *out_val)
{
    if (lookup->count == 0) return false;

    Cook_Mini_Hash_Bucket *bucket = cook_hash__find_bucket(lookup->buckets, lookup->capacity, key);
    if (!bucket || bucket->key != key) return false;

    if (out_val) *out_val = bucket->val;
    return true;
}

void cook_hash_free(Cook_Mini_Hash *lookup)
{
    if (lookup->buckets) free(lookup->buckets);
    lookup->buckets = NULL;
    lookup->capacity = 0;
    lookup->count = 0;
}

#endif /* COOK_IMPLEMENTATION */

#ifdef COOK_STRIP_PREFIX

#define vec_header         cook_vec_header
#define vec_size           cook_vec_size
#define vec_capacity       cook_vec_capacity
#define vec_push           cook_vec_push
#define vec_pop            cook_vec_pop
#define vec_isempty        cook_vec_isempty
#define vec_foreach        cook_vec_foreach
#define vec_free           cook_vec_free
#define vec_reset          cook_vec_reset

#define arr_len            cook_arr_len
#define arr_foreach        cook_arr_foreach

#define hash_set           cook_hash_set
#define hash_get           cook_hash_get
#define hash_free          cook_hash_free

#endif /* COOK_STRIP_PREFIX */

/*
------------------------------------------------------------------------------
This software is available under MIT license.
------------------------------------------------------------------------------
Copyright (c) 2025 Dylaris
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, COOKING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
