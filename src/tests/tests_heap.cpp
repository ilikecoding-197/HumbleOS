// HumbleOS file: tests_heap.cpp
// Purpose: Tests for the heap (using new and delete, of course, but it still appiles)

#include <tests/tests_heap.h>
#include <tests/common.h>

void test_heap_allocation() {
    console::klog("tests", "testing heap allocation...");

    int *p = new int;
    TEST_ASSERT(p != NULL, "failed to allocate int");

    *p = 5;
    TEST_ASSERT(*p == 5, "failed to write to int");

    delete p;

    int *p2 = new int[10];
    TEST_ASSERT(p != NULL, "failed to allocate int array");
    for (int i = 0; i < 10; i++) {
        p2[i] = i;
        TEST_ASSERT(p2[i] == i, "failed to write to int array");
    }
    delete[] p2;

    TEST_PASS("heap_allocation");
}

