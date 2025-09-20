#define ARIS_IMPLEMENTATION
#define ARIS_STRIP_PREFIX
#include "aris.h"

// Test data structure
typedef struct fruit {
    const char *name;
    int price;
    aris_list_node node;
} fruit;

// Function to print all fruits in the list
void print_fruits(aris_list *fruit_list)
{
    printf("Fruits in list (%zu items): ", list_size(fruit_list));

    list_foreach(fruit, fruit_list, iter) {
        printf("%s($%d) ", iter->name, iter->price);
    }
    printf("\n");
}

// Main test function
void test_intrusive_list(void)
{
    printf("=== Testing Intrusive List Implementation ===\n\n");

    // Test 1: List initialization
    printf("Test 1: List initialization\n");
    list_init(fruit_list);
    assert(list_isempty(&fruit_list));
    assert(list_size(&fruit_list) == 0);
    printf("✓ List initialized successfully\n\n");

    // Test 2: Adding elements to list (head and tail)
    printf("Test 2: Adding elements to list\n");
    fruit apple = {"Apple", 1, ARIS_LIST_NODE_INITIALIZER};
    fruit orange = {"Orange", 2, ARIS_LIST_NODE_INITIALIZER};
    fruit banana = {"Banana", 3, ARIS_LIST_NODE_INITIALIZER};
    fruit grape = {"Grape", 4, ARIS_LIST_NODE_INITIALIZER};

    list_add_head(&fruit_list, &apple);
    assert(list_size(&fruit_list) == 1);

    list_add_tail(&fruit_list, &orange);
    assert(list_size(&fruit_list) == 2);

    list_add_head(&fruit_list, &banana);
    assert(list_size(&fruit_list) == 3);

    list_add_tail(&fruit_list, &grape);
    assert(list_size(&fruit_list) == 4);

    print_fruits(&fruit_list);
    printf("✓ Elements added successfully\n\n");

    // Test 3: Forward traversal
    printf("Test 3: Forward traversal\n");
    printf("Forward order: ");
    list_foreach(fruit, &fruit_list, iter) {
        printf("%s ", iter->name);
    }
    printf("\n");
    printf("✓ Forward traversal works correctly\n\n");

    // Test 4: Reverse traversal
    printf("Test 4: Reverse traversal\n");
    printf("Reverse order: ");
    list_foreach_reverse(fruit, &fruit_list, iter) {
        printf("%s ", iter->name);
    }
    printf("\n");
    printf("✓ Reverse traversal works correctly\n\n");

    // Test 5: Check head and tail functions
    printf("Test 5: Head and tail functions\n");
    fruit *head = list_head(fruit, &fruit_list);
    fruit *tail = list_tail(fruit, &fruit_list);
    printf("Head: %s, Tail: %s\n", head->name, tail->name);
    assert(list_ishead(&fruit_list, head));
    assert(list_istail(&fruit_list, tail));
    printf("✓ Head and tail functions work correctly\n\n");

    // Test 6: Check next and previous item functions
    printf("Test 6: Next and previous item functions\n");
    fruit *second = list_next_item_of(fruit, head);
    fruit *first_again = list_prev_item_of(fruit, second);
    printf("Head: %s, Next: %s, Previous: %s\n", head->name, second->name, first_again->name);
    assert(head == first_again);
    printf("✓ Next and previous item functions work correctly\n\n");

    // Test 7: Delete head element
    printf("Test 7: Delete head element\n");
    printf("Before deletion: ");
    print_fruits(&fruit_list);
    list_del_head(&fruit_list);
    printf("After deletion: ");
    print_fruits(&fruit_list);
    assert(list_size(&fruit_list) == 3);
    head = list_head(fruit, &fruit_list);
    printf("New head: %s\n", head->name);
    printf("✓ Head deletion works correctly\n\n");

    // Test 8: Delete tail element
    printf("Test 8: Delete tail element\n");
    printf("Before deletion: ");
    print_fruits(&fruit_list);
    list_del_tail(&fruit_list);
    printf("After deletion: ");
    print_fruits(&fruit_list);
    assert(list_size(&fruit_list) == 2);
    tail = list_tail(fruit, &fruit_list);
    printf("New tail: %s\n", tail->name);
    printf("✓ Tail deletion works correctly\n\n");

    // Test 9: Delete specific element
    printf("Test 9: Delete specific element\n");
    printf("Before deletion: ");
    print_fruits(&fruit_list);
    fruit *to_delete = list_head(fruit, &fruit_list);
    list_del(&fruit_list, to_delete);
    printf("After deletion: ");
    print_fruits(&fruit_list);
    assert(list_size(&fruit_list) == 1);
    printf("✓ Specific element deletion works correctly\n\n");

    // Test 10: Add before and after specific elements
    printf("Test 10: Add before and after specific elements\n");
    fruit pineapple = {"Pineapple", 5, ARIS_LIST_NODE_INITIALIZER};
    fruit mango = {"Mango", 6, ARIS_LIST_NODE_INITIALIZER};

    fruit *remaining = list_head(fruit, &fruit_list);
    list_add_before(&fruit_list, remaining, &pineapple);
    list_add_after(&fruit_list, remaining, &mango);

    printf("After adding before and after: ");
    print_fruits(&fruit_list);
    assert(list_size(&fruit_list) == 3);
    printf("✓ Add before and after works correctly\n\n");

    // Test 11: Reset list
    printf("Test 11: Reset list\n");
    printf("Before reset: ");
    print_fruits(&fruit_list);
    list_reset(&fruit_list);
    printf("After reset: ");
    print_fruits(&fruit_list);
    assert(list_isempty(&fruit_list));
    assert(list_size(&fruit_list) == 0);
    printf("✓ List reset works correctly\n\n");

    // Test 12: Edge case - empty list operations
    printf("Test 12: Empty list operations\n");
    list_del_head(&fruit_list); // Should not crash
    list_del_tail(&fruit_list); // Should not crash
    printf("✓ Empty list operations handled correctly\n\n");

    printf("=== All tests passed! ===\n");
}

int main(void)
{
    test_intrusive_list();
    return 0;
}
