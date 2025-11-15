#define COOK_IMPLEMENTATION
#define COOK_STRIP_PREFIX
#include "cook.h"

int main(void)
{
    int *deque = NULL;

    printf("=== Deque Test Program ===\n\n");

    // Test 1: Initial state
    printf("Test 1: Initial state\n");
    printf("Is deque empty: %s\n", deq_isempty(deque) ? "yes" : "no");
    printf("Deque size: %zu\n", deq_size(deque));
    printf("Deque capacity: %zu\n", deq_capacity(deque));
    printf("\n");

    // Test 2: Push elements to back
    printf("Test 2: Push elements to back\n");
    for (int i = 1; i <= 5; i++) {
        deq_push_back(deque, i);
        printf("Pushed %d to back, size: %zu, capacity: %zu\n",
               i, deq_size(deque), deq_capacity(deque));
    }
    printf("\n");

    // Test 3: Push elements to front
    printf("Test 3: Push elements to front\n");
    for (int i = 10; i <= 12; i++) {
        deq_push_front(deque, i);
        printf("Pushed %d to front, size: %zu, capacity: %zu\n",
               i, deq_size(deque), deq_capacity(deque));
    }
    printf("\n");

    // Test 4: View deque contents
    printf("Test 4: View deque contents\n");
    printf("Deque elements: ");
    deq_foreach(int, deque, it) {
        printf("%d ", *it);
    }
    printf("\n");
    printf("Front element: %d\n", deq_peek_front(deque));
    printf("Rear element: %d\n", deq_peek_back(deque));
    printf("\n");

    // Test 5: Pop elements from front
    printf("Test 5: Pop elements from front\n");
    int front_val;
    while (deq_size(deque) > 5) {
        front_val = deq_peek_front(deque);
        deq_pop_front(deque);
        printf("Popped from front: %d, remaining size: %zu\n", front_val, deq_size(deque));
    }
    printf("\n");

    // Test 6: Pop elements from back
    printf("Test 6: Pop elements from back\n");
    int back_val;
    while (deq_size(deque) > 2) {
        back_val = deq_peek_back(deque);
        deq_pop_back(deque);
        printf("Popped from back: %d, remaining size: %zu\n", back_val, deq_size(deque));
    }
    printf("\n");

    // Test 7: Final state
    printf("Test 7: Final state\n");
    printf("Deque elements: ");
    deq_foreach(int, deque, it) {
        printf("%d ", *it);
    }
    printf("\n");
    printf("Is deque empty: %s\n", deq_isempty(deque) ? "yes" : "no");
    printf("Deque size: %zu\n", deq_size(deque));
    printf("Deque capacity: %zu\n", deq_capacity(deque));
    printf("\n");

    // Test 8: Clear deque completely
    printf("Test 8: Clear deque completely\n");
    while (!deq_isempty(deque)) {
        front_val = deq_peek_front(deque);
        deq_pop_front(deque);
        printf("Popped: %d, remaining size: %zu\n", front_val, deq_size(deque));
    }
    printf("Is deque empty: %s\n", deq_isempty(deque) ? "yes" : "no");
    printf("\n");

    // Test 9: Stress test - many elements
    printf("Test 9: Stress test - many elements\n");
    for (int i = 0; i < 100; i++) {
        if (i % 2 == 0) {
            deq_push_front(deque, i);
        } else {
            deq_push_back(deque, i);
        }
    }
    printf("After adding 100 elements - size: %zu, capacity: %zu\n", deq_size(deque), deq_capacity(deque));

    // Test 10: Iterate through many elements
    printf("Test 10: Iterate through many elements (first 10)\n");
    int count = 0;
    deq_foreach(int, deque, it) {
        // if (count++ < 10) {
        //     printf("%d ", *it);
        // } else {
        //     break;
        // }
        printf("%d ", *it);
    }
    printf("...\n");

    // Cleanup
    deq_free(deque);
    printf("Deque freed\n");

    // Test 11: State after freeing
    printf("Test 11: State after freeing\n");
    printf("Deque pointer: %p\n", (void*)deque);
    printf("Is deque empty: %s\n", deq_isempty(deque) ? "yes" : "no");
    printf("Deque size: %zu\n", deq_size(deque));

    return 0;
}
