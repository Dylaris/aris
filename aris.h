/*
aris.h - v0.04 - Dylaris 2025
===================================================

BRIEF:
  Frequentyly used stuff in C programming implemented using C99.

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
    v0.04 Support data-structure 'mini hash table'
    v0.03 Support data-structure 'doubly instrusive linked list'
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
#include <stdint.h>
#include <ctype.h>
#include <assert.h>

/*
 * dynamic array
 */
typedef struct Aris_Vector_Header {
    size_t size;
    size_t capacity;
} Aris_Vector_Header;

#define aris_vec_header(vec)   ((Aris_Vector_Header*)((char*)(vec) - sizeof(Aris_Vector_Header)))
#define aris_vec_size(vec)     ((vec) ? aris_vec_header(vec)->size : 0)
#define aris_vec_capacity(vec) ((vec) ? aris_vec_header(vec)->capacity : 0)
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
#define aris_vec_foreach(type, vec, iter) for (type *iter = (vec); iter < (vec)+aris_vec_size(vec); iter++)
#define aris_vec_swap_delete(vec, deleted_index)          \
    do {                                                  \
        if (aris_vec_isempty(vec)) break;                 \
        if ((deleted_index) >= aris_vec_size(vec)) break; \
        (vec)[deleted_index] = aris_vec_pop(vec);         \
    } while (0)
void *aris_vec_grow(void *vec, size_t item_size);


/*
 * static array
 */
#define aris_arr_len(arr) (sizeof(arr)/sizeof(arr[0]))
#define aris_arr_foreach(type, arr, iter) for (type *iter = (arr); iter < (arr)+aris_arr_len(arr); iter++)


/*
 * deque
 */
typedef struct Aris_Deque_Header {
    size_t size;
    size_t capacity;
    size_t front;
    size_t rear;
} Aris_Deque_Header;

#define aris_deq_header(deq)   ((Aris_Deque_Header*)((char*)(deq) - sizeof(Aris_Deque_Header)))
#define aris_deq_size(deq)     ((deq) ? aris_deq_header(deq)->size : 0)
#define aris_deq_capacity(deq) ((deq) ? aris_deq_header(deq)->capacity : 0)
#define aris_deq_front(deq)    ((deq) ? aris_deq_header(deq)->front : 0)
#define aris_deq_rear(deq)     ((deq) ? aris_deq_header(deq)->rear : 0)
#define aris_deq_isempty(deq)  ((deq) ? aris_deq_size(deq) == 0 : true)
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
#define aris_deq_peek_back(deq)  (deq)[aris_deq_rear(deq)]
#define aris_deq_peek_front(deq) (deq)[aris_deq_front(deq)]
void *aris_deq_grow(void *deq, size_t item_size);


/*
 * double linked list
 */
typedef struct Aris_List_Node {
    struct Aris_List_Node *prev;
    struct Aris_List_Node *next;
} Aris_List_Node;

typedef struct Aris_List {
    Aris_List_Node head; /* dummy head */
    size_t size;
} Aris_List;

#define ARIS_INSTRUSIVE_LIST_NODE             Aris_List_Node node
#define ARIS_LIST_NODE_INITIALIZER            ((Aris_List_Node){NULL, NULL})
#define ARIS_LIST_INITIALIZER(name)           ((Aris_List){.head = {&name.head, &name.head}, .size = 0})
#define ARIS_LIST_ITEM_INITIALIZER(type, ...) ((type){__VA_ARGS__, .node = LIST_NODE_INITIALIZER})
#define aris_list_init(name)                  Aris_List name = LIST_INITIALIZER(name)
#define aris_list_size(list)                  ((list)->size)
#define aris_list_head(type, list)            list_to_item(type, (list)->head.next)
#define aris_list_tail(type, list)            list_to_item(type, (list)->head.prev)
#define aris_list_node_init(node)           \
    do {                                    \
        if (!node->prev) node->prev = node; \
        if (!node->next) node->next = node; \
    } while (0)
#define aris_list_add_head(list, item)                                        \
    do {                                                                      \
        aris_list_node__add(&(item)->node, &(list)->head, (list)->head.next); \
        (list)->size++;                                                       \
    } while (0)
#define aris_list_add_tail(list, item)                                        \
    do {                                                                      \
        aris_list_node__add(&(item)->node, (list)->head.prev, &(list)->head); \
        (list)->size++;                                                       \
    } while (0)
#define aris_list_add_before(list, item, new_item)                                \
    do {                                                                          \
        aris_list_node__add(&(new_item)->node, (item)->node.prev, &(item)->node); \
        (list)->size++;                                                           \
    } while (0)
#define aris_list_add_after(list, item, new_item)                                 \
    do {                                                                          \
        aris_list_node__add(&(new_item)->node, &(item)->node, (item)->node.next); \
        (list)->size++;                                                           \
    } while (0)
#define aris_list_del_head(list)                                      \
    do {                                                              \
        if ((list)->size == 0) break;                                 \
        Aris_List_Node *head = (list)->head.next;                     \
        aris_list_node__del(&(list)->head, (list)->head.next->next);  \
        head->next = head;                                            \
        head->prev = head;                                            \
        (list)->size--;                                               \
    } while (0)
#define aris_list_del_tail(list);                                     \
    do {                                                              \
        if ((list)->size == 0) break;                                 \
        Aris_List_Node *tail = (list)->head.prev;                     \
        aris_list_node__del((list)->head.prev->prev, &(list)->head);  \
        tail->next = tail;                                            \
        tail->prev = tail;                                            \
        (list)->size--;                                               \
    } while (0)
#define aris_list_del(list, item)                                  \
    do {                                                           \
        if ((list)->size == 0) break;                              \
        aris_list_node__del((item)->node.prev, (item)->node.next); \
        (item)->node.next = &(item)->node;                         \
        (item)->node.prev = &(item)->node;                         \
        (list)->size--;                                            \
    } while (0)
#define aris_list_prev_item_of(type, item) aris_list_to_item(type, (item)->node.prev)
#define aris_list_next_item_of(type, item) aris_list_to_item(type, (item)->node.next)
#define aris_list_to_item(type, ptr)       aris_container_of(ptr, type, node)
#define aris_list_isempty(list)            ((list)->size == 0)
#define aris_list_ishead(list, item)       ((item)->node.prev == &(list)->head)
#define aris_list_istail(list, item)       ((item)->node.next == &(list)->head)
#define aris_list_foreach(type, list, iter)                                            \
    for (Aris_List_Node *current = (list)->head.next, *next = (list)->head.next->next; \
         current != (Aris_List_Node*)(list);                                           \
         current = next, next = next->next)                                            \
        for (type *iter = aris_list_to_item(type, current); iter != NULL; iter = NULL)
#define aris_list_foreach_reverse(type, list, iter)                                    \
    for (Aris_List_Node *current = (list)->head.prev, *prev = (list)->head.prev->prev; \
         current != (Aris_List_Node*)(list);                                           \
         current = prev, prev = prev->prev)                                            \
        for (type *iter = aris_list_to_item(type, current); iter != NULL; iter = NULL)
#define aris_list_reset(list)              \
    do {                                   \
        (list)->head.prev = &(list)->head; \
        (list)->head.next = &(list)->head; \
        (list)->size = 0;                  \
    } while (0)
void aris_list_node__add(Aris_List_Node *new_node, Aris_List_Node *prev_node, Aris_List_Node *next_node);
void aris_list_node__del(Aris_List_Node *prev_node, Aris_List_Node *next_node);

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
typedef struct Aris_Mini_Hash_Bucket {
    uint32_t key;
    uint32_t val;
} Aris_Mini_Hash_Bucket;

typedef struct Aris_Mini_Hash {
    uint32_t count;
    uint32_t capacity;
    Aris_Mini_Hash_Bucket *buckets;
} Aris_Mini_Hash;

#define ARIS_MINI_HASH_MAX_LOAD 0.75

uint32_t fnv_hash(const char *str, size_t length);
bool aris_hash_expand(Aris_Mini_Hash *lookup, size_t new_capacity);
bool aris_hash_set(Aris_Mini_Hash *lookup, uint32_t key, uint32_t val);
bool aris_hash_get(Aris_Mini_Hash *lookup, uint32_t key, uint32_t *out_val);
void aris_hash_free(Aris_Mini_Hash *lookup);


/*
 * string
 */
char *aris_string_tmp_format(const char *fmt, ...);
void aris_string_reverse(char *s);
char *aris_string_tmp_reverse(const char *s);
void aris_string_trim(char *s);
char *aris_string_tmp_trim(const char *s);
bool aris_string_has_prefix(const char *s, const char *prefix);
bool aris_string_has_postfix(const char *s, const char *postfix);


/*
 * log
 */
typedef enum Aris_Log_Level {
    ARIS_LOG_TODO,
    ARIS_LOG_ERROR,
    ARIS_LOG_INFO,
    ARIS_LOG_WARN
} Aris_Log_Level;

#define aris_log(level, fmt, ...)                                                          \
    do {                                                                                   \
        switch (level) {                                                                   \
        case ARIS_LOG_TODO:                                                                \
            fprintf(stderr, "[TODO] "fmt" at %s:%d\n", ##__VA_ARGS__, __FILE__, __LINE__); \
            exit(EXIT_FAILURE);                                                            \
        case ARIS_LOG_ERROR:                                                               \
            fprintf(stderr, "[ERROR] "fmt" at %s:%d\n", ##__VA_ARGS__, __FILE__, __LINE__);\
            exit(EXIT_FAILURE);                                                            \
        case ARIS_LOG_WARN:                                                                \
            fprintf(stdout, "[WARN] "fmt"\n", ##__VA_ARGS__);                              \
            break;                                                                         \
        case ARIS_LOG_INFO:                                                                \
            fprintf(stdout, "[INFO] "fmt"\n", ##__VA_ARGS__);                              \
            break;                                                                         \
        default:                                                                           \
            break;                                                                         \
        }                                                                                  \
    } while (0)


/*
 * file
 */
typedef enum Aris_File_Type {
    ARIS_TEXT_FILE,
    ARIS_BINARY_FILE
} Aris_File_Type ;

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
char *aris_file_read(const char *filename, Aris_File_Type type);


/*
 * compiler
 */
#define ARIS_UNUSED(value) (void)(value)


/*
 * math
 */
#define ARIS_MIN(a, b)          ((a) < (b) ? (a) : (b))
#define ARIS_MAX(a, b)          ((a) > (b) ? (a) : (b))
#define ARIS_SWAP(a, b)         do { (a) ^= (b); (b) ^= (a); (a) ^= (b); } while (0)
#define ARIS_ALIGN_UP(n, k)     (((n) + (k) - 1) & ~((k) - 1))
#define ARIS_ALIGN_DOWN(n, k)   ((n) & ~((k) - 1))
#define ARIS_IS_POWER_OF_TWO(k) ((k) != 0 && ((k) & ((k) - 1)) == 0)


/*
 * variable parameter
 */
#define ARIS_NUM_VARGS(type, ...) (sizeof((type[]){__VA_ARGS__})/sizeof(type))


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
    Aris_Vector_Header *new_header;

    new_capacity = aris_vec_capacity(vec) < 16
                   ? 16 : 2 * aris_vec_capacity(vec);
    alloc_size = sizeof(Aris_Vector_Header) + new_capacity*item_size;

    if (vec) {
        new_header = realloc(aris_vec_header(vec), alloc_size);
        assert(new_header != NULL && "out of memory");
    } else {
        new_header = malloc(alloc_size);
        assert(new_header != NULL && "out of memory");
        new_header->size = 0;
    }
    new_header->capacity = new_capacity;

    return (void*)((char*)new_header + sizeof(Aris_Vector_Header));
}

void *aris_deq_grow(void *deq, size_t item_size)
{
    size_t new_capacity, alloc_size;
    Aris_Deque_Header *new_header;
    void *new_deq;

    new_capacity = aris_deq_capacity(deq) < 16
                   ? 16 : 2 * aris_deq_capacity(deq);
    alloc_size = sizeof(Aris_Deque_Header) + new_capacity*item_size;

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

void aris_list_node__add(Aris_List_Node *new_node, Aris_List_Node *prev_node, Aris_List_Node *next_node)
{
    aris_list_node_init(new_node);
    aris_list_node_init(prev_node);
    aris_list_node_init(next_node);

    prev_node->next = new_node;
    new_node->next = next_node;
    next_node->prev = new_node;
    new_node->prev = prev_node;
}

void aris_list_node__del(Aris_List_Node *prev_node, Aris_List_Node *next_node)
{
    prev_node->next = next_node;
    next_node->prev = prev_node;
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

void aris_string_trim(char *s)
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

char *aris_string_tmp_trim(const char *s)
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
    if (len >= ARIS_TMP_BUFFER_SIZE) len = ARIS_TMP_BUFFER_SIZE - 1;
    snprintf(_tmp_buffer, ARIS_TMP_BUFFER_SIZE, "%.*s", (int)len, start);

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

char *aris_file_read(const char *filename, Aris_File_Type type)
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

static Aris_Mini_Hash_Bucket *aris_hash__find_bucket(Aris_Mini_Hash_Bucket *buckets, uint32_t capacity, uint32_t key)
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

bool aris_hash_expand(Aris_Mini_Hash *lookup, size_t new_capacity)
{
    if (new_capacity <= lookup->capacity) return false;

    Aris_Mini_Hash_Bucket *new_buckets = malloc(sizeof(Aris_Mini_Hash_Bucket)*new_capacity);
    if (!new_buckets) return false;
    for (uint32_t i = 0; i < new_capacity; i++) new_buckets[i].key = 0;

    for (uint32_t i = 0; i < lookup->capacity; i++) {
        Aris_Mini_Hash_Bucket *bucket = &lookup->buckets[i];
        if (bucket->key == 0) continue;
        Aris_Mini_Hash_Bucket *dest = aris_hash__find_bucket(new_buckets, new_capacity, bucket->key);
        dest->key = bucket->key;
        dest->val = bucket->val;
    }

    lookup->capacity = new_capacity;
    if (lookup->buckets) free(lookup->buckets);
    lookup->buckets = new_buckets;

    return true;
}

bool aris_hash_set(Aris_Mini_Hash *lookup, uint32_t key, uint32_t val)
{
    if (lookup->count + 1 > lookup->capacity * ARIS_MINI_HASH_MAX_LOAD) {
        uint32_t new_capacity = lookup->capacity < 16 ? 16 : 2*lookup->capacity;
        if (!aris_hash_expand(lookup, new_capacity)) return false;
    }

    Aris_Mini_Hash_Bucket *bucket = aris_hash__find_bucket(lookup->buckets, lookup->capacity, key);
    if (!bucket) return false;

    bucket->key = key;
    bucket->val = val;
    lookup->count++;

    return true;
}

bool aris_hash_get(Aris_Mini_Hash *lookup, uint32_t key, uint32_t *out_val)
{
    if (lookup->count == 0) return false;

    Aris_Mini_Hash_Bucket *bucket = aris_hash__find_bucket(lookup->buckets, lookup->capacity, key);
    if (!bucket) return false;

    if (out_val) *out_val = bucket->val;
    return true;
}

void aris_hash_free(Aris_Mini_Hash *lookup)
{
    if (lookup->buckets) free(lookup->buckets);
    lookup->buckets = NULL;
    lookup->capacity = 0;
    lookup->count = 0;
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

#define INSTRUSIVE_LIST_NODE  ARIS_INSTRUSIVE_LIST_NODE
#define LIST_NODE_INITIALIZER ARIS_LIST_NODE_INITIALIZER
#define LIST_INITIALIZER      ARIS_LIST_INITIALIZER
#define LIST_ITEM_INITIALIZER ARIS_LIST_ITEM_INITIALIZER
#define list_init            aris_list_init
#define list_size            aris_list_size
#define list_head            aris_list_head
#define list_tail            aris_list_tail
#define list_node_init       aris_list_node_init
#define list_add_head        aris_list_add_head
#define list_add_tail        aris_list_add_tail
#define list_add_before      aris_list_add_before
#define list_add_after       aris_list_add_after
#define list_del_head        aris_list_del_head
#define list_del_tail        aris_list_del_tail
#define list_del             aris_list_del
#define list_prev_item_of    aris_list_prev_item_of
#define list_next_item_of    aris_list_next_item_of
#define list_to_item         aris_list_to_item
#define list_isempty         aris_list_isempty
#define list_ishead          aris_list_ishead
#define list_istail          aris_list_istail
#define list_foreach         aris_list_foreach
#define list_foreach_reverse aris_list_foreach_reverse
#define list_reset           aris_list_reset

#define arr_len            aris_arr_len
#define arr_foreach        aris_arr_foreach

#define string_tmp_format  aris_string_tmp_format
#define string_reverse     aris_string_reverse
#define string_tmp_reverse aris_string_tmp_reverse
#define string_trim        aris_string_trim
#define string_tmp_trim    aris_string_tmp_trim
#define string_has_prefix  aris_string_has_prefix
#define string_has_postfix aris_string_has_postfix

#define hash_expand        aris_hash_expand
#define hash_set           aris_hash_set
#define hash_get           aris_hash_get
#define hash_free          aris_hash_free

#define log                aris_log

#define UNUSED             ARIS_UNUSED

#define MIN                ARIS_MIN
#define MAX                ARIS_MAX
#define SWAP               ARIS_SWAP
#define ALIGN_UP           ARIS_ALIGN_UP
#define ALIGN_DOWN         ARIS_ALIGN_DOWN
#define IS_POWER_OF_TWO    ARIS_IS_POWER_OF_TWO

#define NUM_VARGS          ARIS_NUM_VARGS

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
