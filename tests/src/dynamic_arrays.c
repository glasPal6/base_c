#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "dynamic_array.h"

// Test structure
typedef struct {
    int *items;
    size_t count;
    size_t capacity;
} int_array_t;

// Helper function to verify array state
void verify_array_state(int_array_t *arr, size_t expected_count,
                        size_t expected_capacity) {
    assert(arr->count == expected_count);
    assert(arr->capacity >= expected_capacity);
}

void test_append() {
    printf("Testing append...\n");
    int_array_t arr = {0};

    // Test single append
    da_append(&arr, 42);
    assert(arr.count == 1);
    assert(arr.items[0] == 42);

    // Test multiple appends
    for (int i = 0; i < 300; i++) {
        da_append(&arr, i);
    }
    verify_array_state(&arr, 301, 301);
    assert(arr.items[300] == 299);

    da_free(&arr);
}

void test_append_many() {
    printf("Testing append_many...\n");
    int_array_t arr = {0};
    int values[] = {1, 2, 3, 4, 5};

    da_append_many(&arr, values, 5);
    verify_array_state(&arr, 5, 5);
    for (int i = 0; i < 5; i++) {
        assert(arr.items[i] == values[i]);
    }

    da_free(&arr);
}

void test_remove() {
    printf("Testing remove...\n");
    int_array_t arr = {0};
    for (int i = 0; i < 5; i++) {
        da_append(&arr, i);
    }

    da_remove(&arr, 2);
    verify_array_state(&arr, 4, 5);
    assert(arr.items[2] == 3);  // Verify shift occurred

    da_free(&arr);
}

void test_remove_many() {
    printf("Testing remove_many...\n");
    int_array_t arr = {0};
    for (int i = 0; i < 10; i++) {
        da_append(&arr, i);
    }

    da_remove_many(&arr, 2, 3);
    verify_array_state(&arr, 7, 10);
    assert(arr.items[2] == 5);  // Verify shift occurred

    da_free(&arr);
}

void test_foreach() {
    printf("Testing foreach...\n");
    int_array_t arr = {0};
    for (int i = 0; i < 5; i++) {
        da_append(&arr, i);
    }

    int sum = 0;
    da_foreach(int, item, &arr) { sum += *item; }
    assert(sum == 10);  // 0 + 1 + 2 + 3 + 4

    da_free(&arr);
}

void test_resize() {
    printf("Testing resize...\n");
    int_array_t arr = {0};

    da_resize(&arr, 1000);
    verify_array_state(&arr, 1000, 1000);

    da_resize(&arr, 500);
    verify_array_state(&arr, 500, 1000);  // Capacity shouldn't decrease

    da_free(&arr);
}

void test_find() {
    printf("Testing find...\n");
    int_array_t arr = {0};
    for (int i = 0; i < 5; i++) {
        da_append(&arr, i);
    }

    size_t index = da_find(&arr, 3);
    assert(index == 3);

    index = da_find(&arr, 999);
    assert(index == (size_t)-1);  // Not found

    da_free(&arr);
}

void test_get() {
    printf("Testing get...\n");
    int_array_t arr = {0};
    for (int i = 0; i < 5; i++) {
        da_append(&arr, i);
    }

    assert(da_get(&arr, 2) == 2);
    assert(da_get(&arr, 999) == 0);  // Out of bounds

    da_free(&arr);
}

int main() {
    test_append();
    test_append_many();
    test_remove();
    test_remove_many();
    test_foreach();
    test_resize();
    test_find();
    test_get();

    printf("All tests passed!\n");
    return 0;
}
