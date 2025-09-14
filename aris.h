/*
aris.h - v0.02 - Dylaris 2025
===================================================

BRIEF:
  Frequentyly used stuff in C99. Not compatible with C++.

NOTICE:
  Not compatible with C++.

USAGE:
  In exactly one source file, define the implementation macro
  before including this header:
  ```
    #define ARIS_IMPLEMENTATION
    #include "aris.h"
  ```
  In other files, just include the header without the macro.

HISTORY:
    v0.02 Support data-structure 'deque'

LICENSE:
  See the end of this file for further details.
*/

#ifndef ARIS_H
#define ARIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>

/*
 * dynamic array
 */
typedef struct aris_vector_header {
    size_t size;
    size_t capacity;
} aris_vector_header ;

#define aris_vec_header(vec) \
    ((aris_vector_header*)((char*)(vec) - sizeof(aris_vector_header)))
#define aris_vec_size(vec) ((vec) ? aris_vec_header(vec)->size : 0)
#define aris_vec_capacity(vec)  ((vec) ? aris_vec_header(vec)->capacity : 0)

#define aris_vec_push(vec, item)                               \
    do {                                                       \
        if (aris_vec_size(vec) + 1 > aris_vec_capacity(vec)) { \
            (vec) = aris_vec_grow(vec, sizeof(*vec));          \
        }                                                      \
        (vec)[aris_vec_header(vec)->size++] = (item);          \
    } while (0)
#define aris_vec_pop(vec) ((vec)[--aris_vec_header(vec)->size])

#define aris_vec_isempty(vec) ((vec) ? aris_vec_size(vec) == 0 : false)
#define aris_vec_free(vec)                   \
    do {                                     \
        if (vec) free(aris_vec_header(vec)); \
        (vec) = NULL;                        \
    } while (0)
#define aris_vec_reset(vec)                      \
    do {                                         \
        if (vec) aris_vec_header(vec)->size = 0; \
    } while (0)
#define aris_vec_foreach(type, vec, it) for (type *it = (vec); it < (vec)+aris_vec_size(vec); it++)

void *aris_vec_grow(void *vec, size_t item_size);


/*
 * static array
 */
#define aris_arr_len(arr) (sizeof(arr)/sizeof(arr[0]))
#define aris_arr_foreach(type, arr, it) for (type *it = (arr); it < (arr)+aris_arr_len(arr); it++)


/*
 * hash table
 */


/*
 * deque
 */
typedef struct aris_deque_header {
    size_t size;
    size_t capacity;
    size_t front;
    size_t rear;
} aris_deque_header;

#define aris_deq_header(deq) \
    ((aris_deque_header*)((char*)(deq) - sizeof(aris_deque_header)))
#define aris_deq_size(deq) ((deq) ? aris_deq_header(deq)->size : 0)
#define aris_deq_capacity(deq) ((deq) ? aris_deq_header(deq)->capacity : 0)
#define aris_deq_front(deq) ((deq) ? aris_deq_header(deq)->front : 0)
#define aris_deq_rear(deq) ((deq) ? aris_deq_header(deq)->rear : 0)

#define aris_deq_isempty(deq) ((deq) ? aris_deq_size(deq) == 0 : true)
#define aris_deq_reset(deq)                  \
    do {                                     \
        if (deq) {                           \
            aris_deq_header(deq)->size = 0;  \
            aris_deq_header(deq)->front = 0; \
            aris_deq_header(deq)->rear = 0;  \
        }                                    \
    } while (0)
#define aris_deq_free(deq)                   \
    do {                                     \
        if (deq) free(aris_deq_header(deq)); \
        (deq) = NULL;                        \
    } while (0)
#define aris_deq_foreach(type, deq, it)                                \
    for (size_t idx = aris_deq_front(deq), count = aris_deq_size(deq); \
         count > 0;                                                    \
         idx = (idx+1)%aris_deq_capacity(deq), count--)                \
         for (type *it = &deq[idx]; it != NULL; it = NULL)

#define aris_deq_push_back(deq, item)                                                     \
    do {                                                                                  \
        if (aris_deq_size(deq) + 1 > aris_deq_capacity(deq)) {                            \
            (deq) = aris_deq_grow(deq, sizeof(*deq));                                     \
        }                                                                                 \
        if (aris_deq_size(deq) > 0) {                                                     \
            aris_deq_header(deq)->rear = (aris_deq_rear(deq)+1) % aris_deq_capacity(deq); \
        }                                                                                 \
        (deq)[aris_deq_rear(deq)] = (item);                                               \
        aris_deq_header(deq)->size++;                                                     \
    } while (0)
#define aris_deq_push_front(deq, item)                                          \
    do {                                                                        \
        if (aris_deq_size(deq) + 1 > aris_deq_capacity(deq)) {                  \
            (deq) = aris_deq_grow(deq, sizeof(*deq));                           \
        }                                                                       \
        if (aris_deq_size(deq) > 0) {                                           \
            aris_deq_header(deq)->front = aris_deq_front(deq) == 0              \
                        ? aris_deq_capacity(deq) - 1 : aris_deq_front(deq) - 1; \
        }                                                                       \
        (deq)[aris_deq_front(deq)] = (item);                                    \
        aris_deq_header(deq)->size++;                                           \
    } while (0)

#define aris_deq_pop_front(deq)                                           \
    do {                                                                  \
        if (aris_deq_size(deq) > 0) {                                     \
            aris_deq_header(deq)->size--;                                 \
            if (aris_deq_size(deq) > 0) {                                 \
                aris_deq_header(deq)->front = (aris_deq_front(deq) + 1) % \
                                               aris_deq_capacity(deq);    \
            } else {                                                      \
                aris_deq_header(deq)->front = 0;                          \
                aris_deq_header(deq)->rear = 0;                           \
            }                                                             \
        }                                                                 \
    } while (0)
#define aris_deq_pop_back(deq)                                              \
    do {                                                                    \
        if (aris_deq_size(deq) > 0) {                                       \
            aris_deq_header(deq)->size--;                                   \
            if (aris_deq_size(deq) > 0) {                                   \
                aris_deq_header(deq)->rear = (aris_deq_rear(deq) +          \
                                              aris_deq_capacity(deq) - 1) % \
                                              aris_deq_capacity(deq);       \
            } else {                                                        \
                aris_deq_header(deq)->front = 0;                            \
                aris_deq_header(deq)->rear = 0;                             \
            }                                                               \
        }                                                                   \
    } while (0)

#define aris_deq_peek_back(deq) (deq)[aris_deq_rear(deq)]
#define aris_deq_peek_front(deq) (deq)[aris_deq_front(deq)]

void *aris_deq_grow(void *deq, size_t item_size);


/*
 * double linked list
 */


/*
 * string
 */
char *aris_string_tmp_format(const char *fmt, ...);
void aris_string_reverse(char *s);
char *aris_string_tmp_reverse(const char *s);
bool aris_string_has_prefix(const char *s, const char *prefix);
bool aris_string_has_postfix(const char *s, const char *postfix);


/*
 * log
 */
#define aris_todo(msg)                                                 \
    do {                                                               \
        fprintf(stderr, "%s:%d: TODO: %s\n", __FILE__, __LINE__, msg); \
        abort();                                                       \
    } while(0)
#define aris_unreachable(msg)                                                 \
    do {                                                                      \
        fprintf(stderr, "%s:%d: UNREACHABLE: %s\n", __FILE__, __LINE__, msg); \
        abort();                                                              \
    } while(0)
#define aris_fatal(msg)                                                 \
    do {                                                                \
        fprintf(stderr, "%s:%d: FATAL: %s\n", __FILE__, __LINE__, msg); \
        exit(1);                                                        \
    } while(0)
#define aris_error(msg) \
    fprintf(stderr, "%s:%d: ERROR: %s\n", __FILE__, __LINE__, msg);


/*
 * file
 */
typedef enum aris_file_type {
    ARIS_TEXT_FILE,
    ARIS_BINARY_FILE
} aris_file_type ;

#define aris_file_size(buffer) ((buffer)                              \
                       ? *(size_t*)((char*)(buffer) - sizeof(size_t)) \
                       : (size_t)0)
#define aris_file_free(buffer)                            \
    do {                                                  \
        if (!buffer) break;                               \
        char *p_alloc = (char*)(buffer) - sizeof(size_t); \
        free(p_alloc);                                    \
        (buffer) = NULL;                                  \
    } while (0)
char *aris_file_read(const char *filename, aris_file_type type);


/*
 * compiler
 */
#define aris_unused(value) (void)(value)


/*
 * math
 */
#define aris_min(a, b)          ((a) < (b) ? (a) : (b))
#define aris_max(a, b)          ((a) > (b) ? (a) : (b))
#define aris_swap(a, b)         do { (a) ^= (b); (b) ^= (a); (a) ^= (b); } while (0)
#define aris_align_up(n, k)     (((n) + (k) - 1) & ~((k) - 1))
#define aris_align_down(n, k)   ((n) & ~((k) - 1))
#define aris_is_power_of_two(k) ((k) != 0 && ((k) & ((k) - 1)) == 0)


/*
 * variable parameter
 */
#define aris_num_vargs(type, ...) (sizeof((type[]){__VA_ARGS__})/sizeof(type))


/*
 * memory layout
 */
#define aris_offset_of(type, member) ((size_t)&(((type*)0)->member))
#define aris_container_of(ptr, type, member) \
    ((type*)((char*)(ptr) - aris_offset_of(type, member)))

#endif /* ARIS_H */

#ifdef ARIS_IMPLEMENTATION

void *aris_vec_grow(void *vec, size_t item_size)
{
    size_t new_capacity, alloc_size;
    aris_vector_header *new_header;

    new_capacity = aris_vec_capacity(vec) < 16
                   ? 16 : 2 * aris_vec_capacity(vec);
    alloc_size = sizeof(aris_vector_header) + new_capacity*item_size;

    if (vec) {
        new_header = realloc(aris_vec_header(vec), alloc_size);
        assert(new_header != NULL && "out of memory");
    } else {
        new_header = malloc(alloc_size);
        assert(new_header != NULL && "out of memory");
        new_header->size = 0;
    }
    new_header->capacity = new_capacity;

    return (void*)((char*)new_header + sizeof(aris_vector_header));
}

void *aris_deq_grow(void *deq, size_t item_size)
{
    size_t new_capacity, alloc_size;
    aris_deque_header *new_header;
    void *new_deq;

    new_capacity = aris_deq_capacity(deq) < 16
                   ? 16 : 2 * aris_deq_capacity(deq);
    alloc_size = sizeof(aris_deque_header) + new_capacity*item_size;

    new_header = malloc(alloc_size);
    assert(new_header != NULL && "out of memory");
    new_header->size = aris_deq_size(deq);
    new_header->capacity = new_capacity;
    new_header->front = 0;
    new_header->rear = 0;

    /* The data of the new queue is stored continuously starting from 0. */
    new_deq = (void*)(new_header + 1);
    if (deq) {
        if (aris_deq_front(deq) <= aris_deq_rear(deq)) {
            /* Queue data is continuous in memory. */
            memcpy(new_deq,
                   (char*)deq + aris_deq_front(deq)*item_size,
                   aris_deq_size(deq)*item_size);
        } else {
            /* The queue data is not continuous
               The data is divided into two parts:
               Part One: From front to the end of the array
               Part Two: From the beginning of the array to rear */
            size_t offset = 0;
            memcpy((char*)new_deq + offset*item_size,
                   (char*)deq + aris_deq_front(deq)*item_size,
                   (aris_deq_capacity(deq) - aris_deq_front(deq)) * item_size);
            offset += aris_deq_capacity(deq) - aris_deq_front(deq);
            memcpy((char*)new_deq + offset*item_size,
                   deq,
                   (aris_deq_rear(deq)+1)*item_size);
        }
        new_header->rear = aris_deq_size(deq) - 1;
    }
    aris_deq_free(deq);

    return new_deq;
}


#define ARIS_TMP_BUFFER_SIZE 4096
static char _tmp_buffer[ARIS_TMP_BUFFER_SIZE];

char *aris_string_tmp_format(const char *fmt, ...)
{
    if (!fmt) return NULL;

    va_list args;
    int len;

    va_start(args, fmt);
    len = vsnprintf(_tmp_buffer, ARIS_TMP_BUFFER_SIZE, fmt, args);
    va_end(args);

    if (len < 0) return NULL;
    if (len >= ARIS_TMP_BUFFER_SIZE) {
        _tmp_buffer[ARIS_TMP_BUFFER_SIZE - 1] = '\0';
    }

    return _tmp_buffer;
}

void aris_string_reverse(char *s)
{
    if (!s) return;

    for (size_t i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }
}

char *aris_string_tmp_reverse(const char *s)
{
    if (!s) return NULL;

    size_t pos, len;

    len = strlen(s);
    if (len == 0) {
        _tmp_buffer[0] = '\0';
        return _tmp_buffer;
    }
    if (len >= ARIS_TMP_BUFFER_SIZE) len = ARIS_TMP_BUFFER_SIZE - 1;

    pos = len;
    _tmp_buffer[pos--] = '\0';

    for (size_t i = 0; i < len; i++) {
        _tmp_buffer[pos--] = s[i];
    }

    return _tmp_buffer;
}

bool aris_string_has_prefix(const char *s, const char *prefix)
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

bool aris_string_has_postfix(const char *s, const char *postfix)
{
    if (!s || !postfix) return false;
    if (*postfix == '\0') return true;

    size_t s_len = strlen(s);
    size_t postfix_len = strlen(postfix);

    if (postfix_len > s_len) return false;
    return memcmp(s + s_len - postfix_len, postfix, postfix_len) == 0;
}

char *aris_file_read(const char *filename, aris_file_type type)
{
    FILE *f;
    char *p_alloc;
    char *mode; /* file open mode */
    char *buffer;
    long size;

    switch (type) {
    case ARIS_TEXT_FILE:   mode = "r";  break;
    case ARIS_BINARY_FILE: mode = "rb"; break;
    default:               return NULL;
    }

    f = fopen(filename, mode);
    if (!f) return NULL;

    fseek(f, 0L, SEEK_END);
    size = ftell(f);
    rewind(f);

    p_alloc = malloc(sizeof(size_t) + size + 1);
    if (!p_alloc) return NULL;
    *(size_t*)p_alloc = (size_t)size;

    buffer = p_alloc + sizeof(size_t);
    if (fread(buffer, size, 1, f) != 1) {
        free(p_alloc);
        fclose(f);
        return NULL;
    }
    buffer[size] = '\0';

    fclose(f);
    return buffer;
}

#endif /* ARIS_IMPLEMENTATION */

#ifdef ARIS_STRIP_PREFIX

#define vec_header         aris_vec_header
#define vec_size           aris_vec_size
#define vec_capacity       aris_vec_capacity
#define vec_push           aris_vec_push
#define vec_pop            aris_vec_pop
#define vec_isempty        aris_vec_isempty
#define vec_foreach        aris_vec_foreach
#define vec_free           aris_vec_free
#define vec_reset          aris_vec_reset

#define deq_header         aris_deq_header
#define deq_size           aris_deq_size
#define deq_capacity       aris_deq_capacity
#define deq_front          aris_deq_front
#define deq_rear           aris_deq_rear
#define deq_isempty        aris_deq_isempty
#define deq_reset          aris_deq_reset
#define deq_free           aris_deq_free
#define deq_foreach        aris_deq_foreach
#define deq_push_back      aris_deq_push_back
#define deq_push_front     aris_deq_push_front
#define deq_pop_front      aris_deq_pop_front
#define deq_pop_back       aris_deq_pop_back
#define deq_peek_back      aris_deq_peek_back
#define deq_peek_front     aris_deq_peek_front

#define arr_len            aris_arr_len

#define string_tmp_format  aris_string_tmp_format
#define string_reverse     aris_string_reverse
#define string_tmp_reverse aris_string_tmp_reverse
#define string_has_prefix  aris_string_has_prefix
#define string_has_postfix aris_string_has_postfix

#define todo               aris_todo
#define unreachable        aris_unreachable
#define fatal              aris_fatal
#define error              aris_error

#define unused             aris_unused

#define min                aris_min
#define max                aris_max
#define swap               aris_swap

#define num_vargs          aris_num_vargs

#define offset_of          aris_offset_of
#define container_of       aris_container_of

#define file_size          aris_file_size
#define file_free          aris_file_free
#define file_read          aris_file_read

#endif /* ARIS_STRIP_PREFIX */

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
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
