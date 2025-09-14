#define ARIS_IMPLEMENTATION
#define ARIS_STRIP_PREFIX
#include "aris.h"

int main(void)
{
    int *deque = NULL;

    for (int i = 0; i < 30; i++) {
        deq_push_back(deque, i);
    }
    for (int i = 100; i > 92; i--) {
        deq_push_front(deque, i);
    }

    printf("\nCurrent deque state:\n");
    printf("   Size: %zu, Capacity: %zu\n", deq_size(deque), deq_capacity(deque));
    printf("   Elements: ");
    deq_foreach(int, deque, it) {
        printf("%d ", *it);
    }
    printf("\n");

    printf("   Front element: %d\n", deq_peek_front(deque));
    printf("   Rear element: %d\n", deq_peek_back(deque));

    deq_pop_front(deque);
    deq_pop_back(deque);

    printf("\nAfter popping:\n");
    printf("   Size: %zu, Capacity: %zu\n", deq_size(deque), deq_capacity(deque));
    printf("   Elements: ");
    deq_foreach(int, deque, it) {
        printf("%d ", *it);
    }
    printf("\n");

    deq_reset(deque);
    for (int i = 1000; i < 1005; i++) {
        deq_push_front(deque, i);
    }
    printf("\nAfter reset:\n");
    printf("   Size: %zu, Capacity: %zu\n", deq_size(deque), deq_capacity(deque));
    printf("   Elements: ");
    deq_foreach(int, deque, it) {
        printf("%d ", *it);
    }
    printf("\n");

    deq_free(deque);

    return 0;
}
