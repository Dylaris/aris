/*
cook.h - v0.05 - Dylaris 2025
===================================================

BRIEF:
  Frequentyly used stuff in C programming implemented using C99.

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
    v0.05 Remove data-structure 'doubly instrusive linked list'
    v0.04 Support data-structure 'mini hash table'
    v0.03 Support data-structure 'doubly instrusive linked list'
    v0.02 Support data-structure 'deque'

LICENSE:
  See the end of this file for further details.
*/

#ifndef COOK_H
#define COOK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
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
 * deque
 */
typedef struct Cook_Deque_Header {
    size_t size;
    size_t capacity;
    size_t front;
    size_t rear;
} Cook_Deque_Header;

#define cook_deq_header(deq)   ((Cook_Deque_Header*)((char*)(deq) - sizeof(Cook_Deque_Header)))
#define cook_deq_size(deq)     ((deq) ? cook_deq_header(deq)->size : 0)
#define cook_deq_capacity(deq) ((deq) ? cook_deq_header(deq)->capacity : 0)
#define cook_deq_front(deq)    ((deq) ? cook_deq_header(deq)->front : 0)
#define cook_deq_rear(deq)     ((deq) ? cook_deq_header(deq)->rear : 0)
#define cook_deq_isempty(deq)  ((deq) ? cook_deq_size(deq) == 0 : true)
#define cook_deq_reset(deq)                  \
    do {                                     \
        if (deq) {                           \
            cook_deq_header(deq)->size = 0;  \
            cook_deq_header(deq)->front = 0; \
            cook_deq_header(deq)->rear = 0;  \
        }                                    \
    } while (0)
#define cook_deq_free(deq)                   \
    do {                                     \
        if (deq) free(cook_deq_header(deq)); \
        (deq) = NULL;                        \
    } while (0)
#define cook_deq_foreach(type, deq, it)                                \
    for (size_t idx = cook_deq_front(deq), count = cook_deq_size(deq); \
         count > 0;                                                    \
         idx = (idx+1)%cook_deq_capacity(deq), count--)                \
         for (type *it = &deq[idx]; it != NULL; it = NULL)
#define cook_deq_push_back(deq, item)                                                     \
    do {                                                                                  \
        if (cook_deq_size(deq) + 1 > cook_deq_capacity(deq)) {                            \
            (deq) = cook_deq_grow(deq, sizeof(*deq));                                     \
        }                                                                                 \
        if (cook_deq_size(deq) > 0) {                                                     \
            cook_deq_header(deq)->rear = (cook_deq_rear(deq)+1) % cook_deq_capacity(deq); \
        }                                                                                 \
        (deq)[cook_deq_rear(deq)] = (item);                                               \
        cook_deq_header(deq)->size++;                                                     \
    } while (0)
#define cook_deq_push_front(deq, item)                                          \
    do {                                                                        \
        if (cook_deq_size(deq) + 1 > cook_deq_capacity(deq)) {                  \
            (deq) = cook_deq_grow(deq, sizeof(*deq));                           \
        }                                                                       \
        if (cook_deq_size(deq) > 0) {                                           \
            cook_deq_header(deq)->front = cook_deq_front(deq) == 0              \
                        ? cook_deq_capacity(deq) - 1 : cook_deq_front(deq) - 1; \
        }                                                                       \
        (deq)[cook_deq_front(deq)] = (item);                                    \
        cook_deq_header(deq)->size++;                                           \
    } while (0)
#define cook_deq_pop_front(deq)                                           \
    do {                                                                  \
        if (cook_deq_size(deq) > 0) {                                     \
            cook_deq_header(deq)->size--;                                 \
            if (cook_deq_size(deq) > 0) {                                 \
                cook_deq_header(deq)->front = (cook_deq_front(deq) + 1) % \
                                               cook_deq_capacity(deq);    \
            } else {                                                      \
                cook_deq_header(deq)->front = 0;                          \
                cook_deq_header(deq)->rear = 0;                           \
            }                                                             \
        }                                                                 \
    } while (0)
#define cook_deq_pop_back(deq)                                              \
    do {                                                                    \
        if (cook_deq_size(deq) > 0) {                                       \
            cook_deq_header(deq)->size--;                                   \
            if (cook_deq_size(deq) > 0) {                                   \
                cook_deq_header(deq)->rear = (cook_deq_rear(deq) +          \
                                              cook_deq_capacity(deq) - 1) % \
                                              cook_deq_capacity(deq);       \
            } else {                                                        \
                cook_deq_header(deq)->front = 0;                            \
                cook_deq_header(deq)->rear = 0;                             \
            }                                                               \
        }                                                                   \
    } while (0)
#define cook_deq_peek_back(deq)  (deq)[cook_deq_rear(deq)]
#define cook_deq_peek_front(deq) (deq)[cook_deq_front(deq)]
void *cook_deq_grow(void *deq, size_t item_size);


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
bool cook_hash_expand(Cook_Mini_Hash *lookup, size_t new_capacity);
bool cook_hash_set(Cook_Mini_Hash *lookup, uint32_t key, uint32_t val);
bool cook_hash_get(Cook_Mini_Hash *lookup, uint32_t key, uint32_t *out_val);
void cook_hash_free(Cook_Mini_Hash *lookup);


/*
 * string
 */
char *cook_string_tmp_format(const char *fmt, ...);
void cook_string_reverse(char *s);
char *cook_string_tmp_reverse(const char *s);
void cook_string_trim(char *s);
char *cook_string_tmp_trim(const char *s);
bool cook_string_has_prefix(const char *s, const char *prefix);
bool cook_string_has_postfix(const char *s, const char *postfix);


/*
 * log
 */
typedef enum Cook_Log_Level {
    COOK_LOG_TODO,
    COOK_LOG_ERROR,
    COOK_LOG_INFO,
    COOK_LOG_WARN
} Cook_Log_Level;

#define cook_log(level, fmt, ...)                                                          \
    do {                                                                                   \
        switch (level) {                                                                   \
        case COOK_LOG_TODO:                                                                \
            fprintf(stderr, "[TODO] "fmt" at %s:%d\n", ##__VA_ARGS__, __FILE__, __LINE__); \
            exit(EXIT_FAILURE);                                                            \
        case COOK_LOG_ERROR:                                                               \
            fprintf(stderr, "[ERROR] "fmt" at %s:%d\n", ##__VA_ARGS__, __FILE__, __LINE__);\
            exit(EXIT_FAILURE);                                                            \
        case COOK_LOG_WARN:                                                                \
            fprintf(stdout, "[WARN] "fmt"\n", ##__VA_ARGS__);                              \
            break;                                                                         \
        case COOK_LOG_INFO:                                                                \
            fprintf(stdout, "[INFO] "fmt"\n", ##__VA_ARGS__);                              \
            break;                                                                         \
        default:                                                                           \
            break;                                                                         \
        }                                                                                  \
    } while (0)


/*
 * file
 */
typedef struct Cook_File_Line {
    const char *start;
    size_t length;
    size_t number;
} Cook_File_Line;

typedef struct Cook_File_Header {
    size_t size;
    Cook_File_Line *lines;
    char source[];
} Cook_File_Header;

#define cook_file_header(file) cook_container_of(file, Cook_File_Header, source)
#define cook_file_size(file)   ((file) ? cook_file_header(file)->size : 0)
#define cook_file_lines(file)  ((file) ? cook_file_header(file)->lines : NULL)
#define cook_file_free(file)                            \
    do {                                                \
        if (!(file)) break;                             \
        Cook_File_Header *hdr = cook_file_header(file); \
        if (hdr->lines) cook_vec_free(hdr->lines);      \
        free(hdr);                                      \
        (file) = NULL;                                  \
    } while (0)
#define cook_file_nlines(file) cook_vec_size(cook_file_lines(file))
#define cook_file_foreach(file, iter) \
    for (Cook_File_Line *iter = cook_file_lines(file); iter < cook_file_lines(file)+cook_file_nlines(file); iter++)

const char *cook_file_read(const char *filename);
void cook_file_split(const char *source);


/*
 * compiler
 */
#define COOK_UNUSED(value) (void)(value)


/*
 * math
 */
#define COOK_MIN(a, b)          ((a) < (b) ? (a) : (b))
#define COOK_MAX(a, b)          ((a) > (b) ? (a) : (b))
#define COOK_SWAP(a, b)         do { (a) ^= (b); (b) ^= (a); (a) ^= (b); } while (0)
#define COOK_ALIGN_UP(n, k)     (((n) + (k) - 1) & ~((k) - 1))
#define COOK_ALIGN_DOWN(n, k)   ((n) & ~((k) - 1))
#define COOK_IS_POWER_OF_TWO(k) ((k) != 0 && ((k) & ((k) - 1)) == 0)


/*
 * variable parameter
 */
#define COOK_NUM_VARGS(type, ...) (sizeof((type[]){__VA_ARGS__})/sizeof(type))


/*
 * memory layout
 */
#define cook_offset_of(type, member) ((size_t)&(((type*)0)->member))
#define cook_container_of(ptr, type, member) \
    ((type*)((char*)(ptr) - cook_offset_of(type, member)))

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

void *cook_deq_grow(void *deq, size_t item_size)
{
    size_t new_capacity, alloc_size;
    Cook_Deque_Header *new_header;
    void *new_deq;

    new_capacity = cook_deq_capacity(deq) < 16
                   ? 16 : 2 * cook_deq_capacity(deq);
    alloc_size = sizeof(Cook_Deque_Header) + new_capacity*item_size;

    new_header = malloc(alloc_size);
    assert(new_header != NULL && "out of memory");
    new_header->size = cook_deq_size(deq);
    new_header->capacity = new_capacity;
    new_header->front = 0;
    new_header->rear = 0;

    /* The data of the new queue is stored continuously starting from 0. */
    new_deq = (void*)(new_header + 1);
    if (deq) {
        if (cook_deq_front(deq) <= cook_deq_rear(deq)) {
            /* Queue data is continuous in memory. */
            memcpy(new_deq,
                   (char*)deq + cook_deq_front(deq)*item_size,
                   cook_deq_size(deq)*item_size);
        } else {
            /* The queue data is not continuous
               The data is divided into two parts:
               Part One: From front to the end of the array
               Part Two: From the beginning of the array to rear */
            size_t offset = 0;
            memcpy((char*)new_deq + offset*item_size,
                   (char*)deq + cook_deq_front(deq)*item_size,
                   (cook_deq_capacity(deq) - cook_deq_front(deq)) * item_size);
            offset += cook_deq_capacity(deq) - cook_deq_front(deq);
            memcpy((char*)new_deq + offset*item_size,
                   deq,
                   (cook_deq_rear(deq)+1)*item_size);
        }
        new_header->rear = cook_deq_size(deq) - 1;
    }
    cook_deq_free(deq);

    return new_deq;
}

#define COOK_TMP_BUFFER_SIZE 4096
static char _tmp_buffer[COOK_TMP_BUFFER_SIZE];

char *cook_string_tmp_format(const char *fmt, ...)
{
    if (!fmt) return NULL;

    va_list args;
    int len;

    va_start(args, fmt);
    len = vsnprintf(_tmp_buffer, COOK_TMP_BUFFER_SIZE, fmt, args);
    va_end(args);

    if (len < 0) return NULL;
    if (len >= COOK_TMP_BUFFER_SIZE) {
        _tmp_buffer[COOK_TMP_BUFFER_SIZE - 1] = '\0';
    }

    return _tmp_buffer;
}

void cook_string_reverse(char *s)
{
    if (!s) return;

    for (size_t i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }
}

char *cook_string_tmp_reverse(const char *s)
{
    if (!s) return NULL;

    size_t pos, len;

    len = strlen(s);
    if (len == 0) {
        _tmp_buffer[0] = '\0';
        return _tmp_buffer;
    }
    if (len >= COOK_TMP_BUFFER_SIZE) len = COOK_TMP_BUFFER_SIZE - 1;

    pos = len;
    _tmp_buffer[pos--] = '\0';

    for (size_t i = 0; i < len; i++) {
        _tmp_buffer[pos--] = s[i];
    }

    return _tmp_buffer;
}

void cook_string_trim(char *s)
{
    if (!s) return;

    const char *start, *end;
    size_t len;

    start = s;
    end = s + strlen(s) - 1;

    while (true) {
        if (isspace(*start)) start++;
        if (isspace(*end)) end--;
        if (!isspace(*start) && !isspace(*end)) break;
        if (start > end) return;
    }

    len = end - start + 1;
    for (size_t i = 0; i < len; i++) {
        s[i] = *start++;
    }
    s[len] = '\0';
}

char *cook_string_tmp_trim(const char *s)
{
    if (!s) return NULL;

    const char *start, *end;
    size_t len;

    start = s;
    end = s + strlen(s) - 1;

    while (true) {
        if (isspace(*start)) start++;
        if (isspace(*end)) end--;
        if (!isspace(*start) && !isspace(*end)) break;
        if (start > end) return NULL;
    }

    len = end - start + 1;
    if (len >= COOK_TMP_BUFFER_SIZE) len = COOK_TMP_BUFFER_SIZE - 1;
    snprintf(_tmp_buffer, COOK_TMP_BUFFER_SIZE, "%.*s", (int)len, start);

    return _tmp_buffer;
}

bool cook_string_has_prefix(const char *s, const char *prefix)
{
    if (!s || !prefix) return false;
    if (*prefix == '\0') return true;

    while (*prefix) {
        if (*s != *prefix) return false;
        if (*s == '\0') return false;
        s++;
        prefix++;
    }

    return true;
}

bool cook_string_has_postfix(const char *s, const char *postfix)
{
    if (!s || !postfix) return false;
    if (*postfix == '\0') return true;

    size_t s_len = strlen(s);
    size_t postfix_len = strlen(postfix);

    if (postfix_len > s_len) return false;
    return memcmp(s + s_len - postfix_len, postfix, postfix_len) == 0;
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

uint32_t fnv_hash(const char *str, size_t length)
{
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < length; i++) {
        hash ^= (uint8_t)str[i];
        hash *= 16777619;
    }
    return hash;
}

bool cook_hash_expand(Cook_Mini_Hash *lookup, size_t new_capacity)
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

bool cook_hash_set(Cook_Mini_Hash *lookup, uint32_t key, uint32_t val)
{
    if (lookup->count + 1 > lookup->capacity * COOK_MINI_HASH_MAX_LOAD) {
        uint32_t new_capacity = lookup->capacity < 16 ? 16 : 2*lookup->capacity;
        if (!cook_hash_expand(lookup, new_capacity)) return false;
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

const char *cook_file_read(const char *filename)
{
#define return_defer(flag) do { ok = flag; goto defer; } while (0)
    FILE *f = NULL;
    Cook_File_Header *header = NULL;
    long size;
    bool ok = true;

    f = fopen(filename, "rb");
    if (!f) return NULL;

    fseek(f, 0L, SEEK_END);
    size = ftell(f);
    rewind(f);

    header = malloc(sizeof(Cook_File_Header) + size + 1);
    if (!header) return_defer(false);
    header->size = size;
    header->lines = NULL;

    if (fread(header->source, size, 1, f) != 1) return_defer(false);
    header->source[size] = '\0';

defer:
    if (f) fclose(f);
    if (!ok) {
        if (header) free(header);
        return NULL;
    }
    return header->source;
#undef return_defer
}

void cook_file_split(const char *source)
{
    const char *start = source;
    const char *current = start;
    size_t line_number = 0;
    Cook_File_Line *lines = cook_file_header(source)->lines;

    while (*current) {
        if (*current == '\n') {
            Cook_File_Line line = {
                .start  = start,
                .length = current - start,
                .number = ++line_number
            };
            if (current - 1 >= start && current[-1] == '\r') line.length--;
            cook_vec_push(lines, line);
            start = current + 1;
        }
        current++;
    }

    cook_file_header(source)->lines = lines;
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

#define deq_header         cook_deq_header
#define deq_size           cook_deq_size
#define deq_capacity       cook_deq_capacity
#define deq_front          cook_deq_front
#define deq_rear           cook_deq_rear
#define deq_isempty        cook_deq_isempty
#define deq_reset          cook_deq_reset
#define deq_free           cook_deq_free
#define deq_foreach        cook_deq_foreach
#define deq_push_back      cook_deq_push_back
#define deq_push_front     cook_deq_push_front
#define deq_pop_front      cook_deq_pop_front
#define deq_pop_back       cook_deq_pop_back
#define deq_peek_back      cook_deq_peek_back
#define deq_peek_front     cook_deq_peek_front

#define arr_len            cook_arr_len
#define arr_foreach        cook_arr_foreach

#define string_tmp_format  cook_string_tmp_format
#define string_reverse     cook_string_reverse
#define string_tmp_reverse cook_string_tmp_reverse
#define string_trim        cook_string_trim
#define string_tmp_trim    cook_string_tmp_trim
#define string_has_prefix  cook_string_has_prefix
#define string_has_postfix cook_string_has_postfix

#define hash_expand        cook_hash_expand
#define hash_set           cook_hash_set
#define hash_get           cook_hash_get
#define hash_free          cook_hash_free

#define log                cook_log

#define UNUSED             COOK_UNUSED

#define MIN                COOK_MIN
#define MAX                COOK_MAX
#define SWAP               COOK_SWAP
#define ALIGN_UP           COOK_ALIGN_UP
#define ALIGN_DOWN         COOK_ALIGN_DOWN
#define IS_POWER_OF_TWO    COOK_IS_POWER_OF_TWO

#define NUM_VARGS          COOK_NUM_VARGS

#define offset_of          cook_offset_of
#define container_of       cook_container_of

#define file_header        cook_file_header
#define file_size          cook_file_size
#define file_lines         cook_file_lines
#define file_free          cook_file_free
#define file_nlines        cook_file_nlines
#define file_foreach       cook_file_foreach
#define file_read          cook_file_read
#define file_split         cook_file_split

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
