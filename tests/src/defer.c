#include "defer.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

// Global variables to track execution flow
int execution_counter = 0;
int defer_executions[10] = {0};

void record_execution(int id) { defer_executions[execution_counter++] = id; }

void test_basic_defer() {
    printf("Testing basic defer...\n");

    execution_counter = 0;
    {
        defer({ record_execution(1); });
        record_execution(0);
    }

    assert(defer_executions[0] == 0);
    assert(defer_executions[1] == 1);
}

void test_multiple_defers() {
    printf("Testing multiple defers...\n");

    execution_counter = 0;
    {
        defer({ record_execution(3); });
        defer({ record_execution(2); });
        defer({ record_execution(1); });
        record_execution(0);
    }

    // Check that defers executed in reverse order
    assert(defer_executions[0] == 0);
    assert(defer_executions[1] == 1);
    assert(defer_executions[2] == 2);
    assert(defer_executions[3] == 3);
}

void test_nested_defers() {
    printf("Testing nested defers...\n");

    execution_counter = 0;
    {
        defer({
            record_execution(2);
            defer({ record_execution(3); });
        });
        defer({ record_execution(1); });
        record_execution(0);
    }

    assert(defer_executions[0] == 0);
    assert(defer_executions[1] == 1);
    assert(defer_executions[2] == 2);
    assert(defer_executions[3] == 3);
}

int test_with_return() {
    printf("Testing defer with return...\n");

    execution_counter = 0;
    {
        defer({ record_execution(1); });
        record_execution(0);
        return 42;  // defer should still execute
    }
}

void test_with_variables() {
    printf("Testing defer with variables...\n");

    execution_counter = 0;
    {
        int x = 4;
        char* msg = "H";
        defer({
            assert(x == 42);
            assert(strcmp(msg, "Hello") == 0);
            record_execution(1);
        });
        x = 42;
        msg = "Hello";
        record_execution(0);
    }

    assert(defer_executions[0] == 0);
    assert(defer_executions[1] == 1);
}

void test_resource_cleanup() {
    printf("Testing resource cleanup...\n");

    execution_counter = 0;
    FILE* file = NULL;
    {
        file = tmpfile();
        assert(file != NULL);
        defer({
            fclose(file);
            file = NULL;
            record_execution(1);
        });

        // Use the file
        fputs("test", file);
        record_execution(0);
    }

    assert(file == NULL);  // File should be closed by defer
    assert(defer_executions[0] == 0);
    assert(defer_executions[1] == 1);
}

int main() {
    test_basic_defer();
    test_multiple_defers();
    test_nested_defers();

    // Test with return
    int ret = test_with_return();
    assert(ret == 42);
    assert(defer_executions[0] == 0);
    assert(defer_executions[1] == 1);

    test_with_variables();
    test_resource_cleanup();

    printf("All tests passed!\n");
    return 0;
}
